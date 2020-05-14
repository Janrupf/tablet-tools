#pragma once

#include "tablet-tools/uefi-bootengine/efi/protocol/efi_usb.hpp"
#include "tablet-tools/uefi-bootengine/stddefs.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    class UsbIoProtocol {
    public:
        using ProtocolType = EFI_USB_IO_PROTOCOL;

    private:
        EFI_USB_IO_PROTOCOL *interface;

    public:
        static EFI_GUID *get_guid();

        explicit UsbIoProtocol(EFI_USB_IO_PROTOCOL *interface);
        EFI_STATUS reset();
    };
}