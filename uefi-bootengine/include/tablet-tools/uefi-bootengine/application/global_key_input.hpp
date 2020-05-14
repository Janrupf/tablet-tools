#pragma once

#include "tablet-tools/uefi-bootengine/std/vector.hpp"
#include "tablet-tools/uefi-bootengine/efi/protocol_handle.hpp"
#include "tablet-tools/uefi-bootengine/efi/simple_text_input_protocol.hpp"
#include "tablet-tools/uefi-bootengine/efi/event_manager.hpp"

namespace tablet_tools::uefi_bootengine {
    class GlobalKeyInput {
    private:
        std::Vector<efi_std::OpenedProtocolHandle<efi_std::SimpleTextInputProtocol>> opened_protocols;
        void handle_key_event(EFI_INPUT_KEY key);

    public:
        explicit GlobalKeyInput();
        void register_events(efi_std::EventManager &event_manager);
    };
}