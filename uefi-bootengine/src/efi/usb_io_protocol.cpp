#include "tablet-tools/uefi-bootengine/efi/usb_io_protocol.hpp"

#include "tablet-tools/uefi-bootengine/environment.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    UsbIoProtocol::UsbIoProtocol(EFI_USB_IO_PROTOCOL *interface) : interface(interface) {}

    EFI_GUID *UsbIoProtocol::get_guid() {
        return &gEfiUsbIoProtocol;
    }

    EFI_STATUS UsbIoProtocol::reset() {
        return Environment::uefi_call(interface->UsbPortReset,interface);
    }
}