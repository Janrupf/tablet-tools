#pragma once

#include "tablet-tools/uefi-bootengine/efi/protocol/efi_usb2_hc.hpp"
#include "tablet-tools/uefi-bootengine/bitmask.hpp"
#include "tablet-tools/uefi-bootengine/result.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    enum class Usb2HcProtocolResetFlags {
        RESET_GLOBAL = EFI_USB_HC_RESET_GLOBAL,
        RESET_HOST_CONTROLLER = EFI_USB_HC_RESET_HOST_CONTROLLER,
        RESET_GLOBAL_WITH_DEBUG = EFI_USB_HC_RESET_GLOBAL_WITH_DEBUG,
        RESET_HOST_WITH_DEBUG = EFI_USB_HC_RESET_HOST_WITH_DEBUG
    };

    class Usb2HcProtocol {
    public:
        using ProtocolType = EFI_USB2_HC_PROTOCOL;

    private:
        EFI_USB2_HC_PROTOCOL *interface;

    public:
        static EFI_GUID *get_guid();

        explicit Usb2HcProtocol(EFI_USB2_HC_PROTOCOL *interface);
        EFI_STATUS reset(Usb2HcProtocolResetFlags flags);
        Result<EFI_USB_HC_STATE, EFI_STATUS> get_state();
        EFI_STATUS set_state(EFI_USB_HC_STATE state);
    };
}
