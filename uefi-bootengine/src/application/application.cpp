#include "tablet-tools/uefi-bootengine/application/application.hpp"

#include "tablet-tools/uefi-bootengine/efi/event_manager.hpp"
#include "tablet-tools/uefi-bootengine/efi/protocol/efi_usb.hpp"
#include "tablet-tools/uefi-bootengine/efi/protocol_manager.hpp"
#include "tablet-tools/uefi-bootengine/efi/simple_text_input_protocol.hpp"
#include "tablet-tools/uefi-bootengine/std/string.hpp"

namespace tablet_tools::uefi_bootengine {
    EFI_STATUS main() {
        efi_std::EventManager event_manager;

        std::cout << u"Starting up UEFI bootengine from tablet tools project" << std::endl;

        std::cout << u"Looping event manager" << std::endl;
        event_manager.loop();

        Environment::uefi_call(gBS->Stall, 1000 * 1000 * 5);

        return EFI_SUCCESS;
    }

    void key_read(efi_std::OpenedProtocolHandle<efi_std::SimpleTextInputProtocol> protocol) {
        auto maybe_key = protocol->read_key_stroke();
        if(maybe_key) {
            std::cout << u"Read key: '" << maybe_key.value().UnicodeChar << u"' (" << maybe_key.value().ScanCode << u")" << std::endl;
        } else {
            std::cerr << u"Error while reading key: " << Environment::status_to_string(maybe_key.error()) << std::endl;
        }
    }
}