#include "tablet-tools/uefi-bootengine/efi/event_manager.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    EventManager::EventManager() : should_loop(false), error_event(nullptr) {
        EFI_STATUS status = Environment::uefi_call(
                gBS->CreateEvent,
                0,
                0,
                nullptr,
                nullptr,
                &error_event
                );
        if(EFI_ERROR(status)) {
            std::cerr << u"Failed to create efi event: " << Environment::status_to_string(status) << std::endl;
        } else {
            register_listener(error_event, std::Function<void()>([this] {
                auto event = stacked_errors.remove_first();
                for(auto &listener : error_listeners) {
                    listener(event);
                }
            }));
        }
    }

    EventManager::~EventManager() {
        Environment::uefi_call(gBS->CloseEvent, error_event);
    }

    void EventManager::register_listener(EFI_EVENT event, std::Function<void()> listener) {
        if(listeners.contains(event)) {
            listeners.get(event)->add(std::move(listener));
        } else {
            std::Vector<std::Function<void()>> vector;
            vector.add(std::move(listener));
            listeners.put(event, std::move(vector));
        }
    }

    void EventManager::signal_error(ErrorEvent event) {
        stacked_errors.append(std::move(event));
        EFI_STATUS status = Environment::uefi_call(gBS->SignalEvent, error_event);
        if(EFI_ERROR(status)) {
            std::cerr << u"Failed to signal error event: " << Environment::status_to_string(status) << std::endl;
        }
    }

    void EventManager::on_error(std::Function<void(const ErrorEvent &event)> listener) {
        error_listeners.add(std::move(listener));
    }

    EFI_STATUS EventManager::loop() {
        auto events = listeners.keys();
        if(events.empty()) {
            return EFI_NOT_READY;
        }

        should_loop = true;

        EFI_STATUS status;
        while(should_loop) {
            UINTN event_index = 0;

            EFI_EVENT _events[1] = {gST->ConIn->WaitForKey};

            status = Environment::uefi_call(
                    gBS->WaitForEvent,
                    1,
                    _events,
                    &event_index
                    );
            if(status != EFI_SUCCESS) {
                return status;
            }

            std::Vector<std::Function<void()>> *event_listeners = listeners.get(events.get(event_index));
            if(event_listeners) {
                for(auto &fn : *event_listeners) {
                    fn();
                }
            }
        }

        return EFI_SUCCESS;
    }
}