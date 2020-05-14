#include "tablet-tools/uefi-bootengine/application/global_key_input.hpp"
#include "tablet-tools/uefi-bootengine/efi/protocol_manager.hpp"
#include "tablet-tools/uefi-bootengine/efi/simple_text_input_protocol.hpp"

namespace tablet_tools::uefi_bootengine {
    GlobalKeyInput::GlobalKeyInput() {
        auto maybe_opened_protocols = efi_std::find_protocols<efi_std::SimpleTextInputProtocol>()
                .map([](const auto &protocols) {
                    std::cout << u"Found " << protocols.size() << u" input protocols." << std::endl;
                    return protocols.map([](const auto &handle) { return handle.open(); });
                });

        if(maybe_opened_protocols) {
            for(const auto &maybe_opened_protocol : maybe_opened_protocols.value()) {
                if(maybe_opened_protocol) {
                    opened_protocols.add(maybe_opened_protocol.value());
                } else {
                    std::cerr << u"Failed to open one input protocol: "
                              << Environment::status_to_string(maybe_opened_protocol.error()) << std::endl;
                }
            }
        } else {
            std::cerr << u"Failed to open input protocols: "
                      << Environment::status_to_string(maybe_opened_protocols.error()) << std::endl;
        }
    }

    void GlobalKeyInput::register_events(efi_std::EventManager &event_manager) {
        for(auto opened_protocol : opened_protocols) {
            event_manager.register_listener(opened_protocol->wait_for_key_event(),
                    std::Function<void()>([this, opened_protocol] () mutable {
                auto maybe_read = opened_protocol->read_key_stroke();
                if(maybe_read) {
                    handle_key_event(maybe_read.value());
                }
            }));
        }
    }

    void GlobalKeyInput::handle_key_event(EFI_INPUT_KEY key) {

    }
}