#include <iostream>

#include "tablet-tools/usb-control/libusb_plus_plus.hpp"

namespace tablet_tools::usb_control {
    int main(int argc, const char **argv) {
        LibUsbInstance libusb;
        libusb.run(libusb_set_option, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);

        libusb_device **devices;

        libusb_device *target_device = nullptr;
        libusb_device_descriptor target_descriptor{};

        ssize_t device_count = libusb.run(libusb_get_device_list, &devices);
        for(size_t i = 0; i < device_count; i++) {
            libusb.run(libusb_get_device_descriptor, devices[i], &target_descriptor);
            if(target_descriptor.idVendor == 0x17ef && target_descriptor.idProduct == 0x77a5) {
                target_device = devices[i];
                libusb_ref_device(target_device);
                break;
            }
        }

        libusb_device_handle *target_handle = nullptr;
        if(target_device) {
            libusb.run(libusb_open, target_device, &target_handle);
        }

        libusb_free_device_list(devices, true);

        if(!target_device) {
            std::cerr << "Failed to find target tablet!" << std::endl;
            return 1;
        }

        std::cout << std::hex << std::uppercase;
        std::cout << "Found target tablet:" << std::endl;
        std::cout << "  Bus id: 0x" << (int) libusb_get_bus_number(target_device) << std::endl;
        std::cout << "  Device address: 0x" << (int) libusb_get_device_address(target_device) << std::endl;
        std::cout << std::dec << std::nouppercase;

        libusb.run(libusb_claim_interface, target_handle, 0);


        libusb_release_interface(target_handle, 0);
        libusb_close(target_handle);

        return 0;
    }
}

int main(int argc, const char **argv) {
    return tablet_tools::usb_control::main(argc, argv);
}