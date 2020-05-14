#include "tablet-tools/usb-control/libusb_plus_plus.hpp"

namespace tablet_tools::usb_control {
    LibUsbError::LibUsbError(int libusb_error) : libusb_error(libusb_error), message(libusb_error_name(libusb_error)) {}

    const char *LibUsbError::what() const noexcept {
        return message.c_str();
    }

    LibUsbInstance::LibUsbInstance() : context(nullptr) {
        int libusb_error = libusb_init(&context);
        if(libusb_error != 0) {
            throw LibUsbError(libusb_error);
        }
    }

    LibUsbInstance::~LibUsbInstance() {
        libusb_exit(context);
    }
}