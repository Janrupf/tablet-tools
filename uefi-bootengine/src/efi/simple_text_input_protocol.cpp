#include "tablet-tools/uefi-bootengine/efi/simple_text_input_protocol.hpp"

#include "tablet-tools/uefi-bootengine/environment.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    SimpleTextInputProtocol::SimpleTextInputProtocol(EFI_SIMPLE_TEXT_IN_PROTOCOL *interface) : interface(interface) {}

    EFI_STATUS SimpleTextInputProtocol::reset(bool extended_verification) {
        return Environment::uefi_call(interface->Reset, interface, extended_verification);
    }

    Result<EFI_INPUT_KEY, EFI_STATUS> SimpleTextInputProtocol::read_key_stroke() {
        EFI_INPUT_KEY key;
        EFI_STATUS status = Environment::uefi_call(
                interface->ReadKeyStroke,
                interface,
                &key
                );
        if(status != EFI_SUCCESS) {
            return Result<EFI_INPUT_KEY, EFI_STATUS>::make_error(status);
        } else {
            return Result<EFI_INPUT_KEY, EFI_STATUS>::make_ok(key);
        }
    }

    EFI_EVENT SimpleTextInputProtocol::wait_for_key_event() const {
        return interface->WaitForKey;
    }

    EFI_GUID *SimpleTextInputProtocol::get_guid() {
        return &gEfiSimpleTextInProtocolGuid;
    }
}