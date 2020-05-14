#include "tablet-tools/uefi-bootengine/efi/usb_hc2_protocol.hpp"
#include "tablet-tools/uefi-bootengine/environment.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    EFI_GUID *Usb2HcProtocol::get_guid() {
        return &gEfiUsb2HcProtocol;
    }

    Usb2HcProtocol::Usb2HcProtocol(EFI_USB2_HC_PROTOCOL *interface) : interface(interface) {}

    EFI_STATUS Usb2HcProtocol::reset(Usb2HcProtocolResetFlags flags) {
        return Environment::uefi_call(interface->Reset, interface, AS_BITMASK(flags));
    }

    Result<EFI_USB_HC_STATE, EFI_STATUS> Usb2HcProtocol::get_state() {
        EFI_USB_HC_STATE state;
        EFI_STATUS status = Environment::uefi_call(interface->GetState, interface, &state);
        if(status != EFI_SUCCESS) {
            return Result<EFI_USB_HC_STATE, EFI_STATUS>::make_error(status);
        }

        return Result<EFI_USB_HC_STATE, EFI_STATUS>::make_ok(state);
    }

    EFI_STATUS Usb2HcProtocol::set_state(EFI_USB_HC_STATE state) {
        return Environment::uefi_call(interface->SetState, interface, state);
    }
}