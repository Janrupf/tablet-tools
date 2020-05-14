#pragma once

#include "tablet-tools/uefi-bootengine/std/function.hpp"
#include "tablet-tools/uefi-bootengine/std/hash_map.hpp"
#include "tablet-tools/uefi-bootengine/std/linked_list.hpp"
#include "tablet-tools/uefi-bootengine/std/string.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    struct ErrorEvent {
        EFI_STATUS status;
        std::String message;
    };

    class EventManager {
    private:
        std::HashMap<EFI_EVENT, std::Vector<std::Function<void()>>> listeners;
        std::Vector<std::Function<void(const ErrorEvent &event)>> error_listeners;
        std::LinkedList<ErrorEvent> stacked_errors;

        bool should_loop;
        EFI_EVENT error_event;

    public:
        explicit EventManager();
        ~EventManager();

        void register_listener(EFI_EVENT event, std::Function<void()> listener);
        void signal_error(ErrorEvent event);
        void on_error(std::Function<void(const ErrorEvent &event)> listener);
        EFI_STATUS loop();
    };
}