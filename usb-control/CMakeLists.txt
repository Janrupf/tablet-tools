# usb-control executable, helps sending commands
# to the device using usb

# We require libusb-1.0
pkg_check_modules(LIBUSB libusb-1.0)

set(USB_CONTROL_SOURCE
        src/main.cpp
        src/libusb_plus_plus.cpp)

add_executable(usb-control ${USB_CONTROL_SOURCE})
target_include_directories(usb-control PUBLIC include ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(usb-control PUBLIC ${LIBUSB_LIBRARIES})