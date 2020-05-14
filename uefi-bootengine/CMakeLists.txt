# uefi-bootengine executable, run as UEFI application
# on the target device

set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")
set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "")

set(UEFI_BOOTENGINE_SOURCE
        src/application/application.cpp
        src/application/global_key_input.cpp

        src/efi/protocol/efi_usb.cpp
        src/efi/protocol/efi_usb2_hc.cpp

        src/efi/event_manager.cpp
        src/efi/protocol_manager.cpp
        src/efi/simple_text_input_protocol.cpp
        src/efi/usb_io_protocol.cpp
        src/efi/usb_hc2_protocol.cpp

        src/std/std_stream.cpp
        src/std/string.cpp

        src/environment.cpp
        src/main.cpp
        src/stdlib.cpp
        )

add_library(uefi-bootengine-lib SHARED ${UEFI_BOOTENGINE_SOURCE})
set_target_properties(uefi-bootengine-lib PROPERTIES LINKER_LANGUAGE C)
target_compile_options(uefi-bootengine-lib PUBLIC
        -ffreestanding
        -nostdinc
        -nostdinc++
        -nostdlib
        -fno-exceptions
        -fno-rtti)
target_link_options(uefi-bootengine-lib PUBLIC -nostdlib)
target_include_directories(uefi-bootengine-lib PUBLIC include)
target_link_libraries(uefi-bootengine-lib PUBLIC efi)
create_efi_image(uefi-bootengine-lib uefi-bootengine)

set(UEFI_RUN_DIR "${CMAKE_CURRENT_BINARY_DIR}/uefi-run")
file(MAKE_DIRECTORY "${UEFI_RUN_DIR}")
# configure_file("/home/jannis/Coding/Native/edk2/Build/OvmfX64/DEBUG_GCC92/FV/OVMF.fd" "${UEFI_RUN_DIR}/bios.bin" COPYONLY)
configure_file("/usr/share/ovmf/x64/OVMF_CODE.fd" "${UEFI_RUN_DIR}/bios.bin" COPYONLY)

set(UEFI_DISK_DIR "${UEFI_RUN_DIR}/hda-contents")
file(WRITE "${UEFI_DISK_DIR}/startup.nsh"
        "echo -on\n"
        "fs0:\n"
        ".\\EFI\\BOOT\\BOOTx64.EFI")

get_target_property(UEFI_BOOT_FILE_SOURCE uefi-bootengine-lib EFI_IMAGE_FILE)
set(UEFI_BOOT_DIR "${UEFI_DISK_DIR}/EFI/BOOT")

add_custom_target(uefi-copy-boot DEPENDS uefi-bootengine-lib)
add_custom_command(
        TARGET uefi-copy-boot
        COMMAND "${CMAKE_COMMAND}" -E copy "${UEFI_BOOT_FILE_SOURCE}" "${UEFI_BOOT_DIR}/BOOTx64.EFI"
)

set(COMMON_QEMU_COMMAND
        qemu-system-x86_64
        --bios bios.bin
        -drive "file=fat:rw:${UEFI_DISK_DIR},format=raw"
        -enable-kvm
        )

add_custom_target(run-qemu DEPENDS uefi-copy-boot)
add_custom_command(
        TARGET run-qemu
        COMMAND ${COMMON_QEMU_COMMAND}
        WORKING_DIRECTORY "${UEFI_RUN_DIR}"
)

add_custom_target(debug-qemu DEPENDS uefi-copy-boot)
add_custom_command(
        TARGET debug-qemu
        COMMAND ${COMMON_QEMU_COMMAND}
                -gdb tcp::1234
                -serial "file:${TABLET_TOOLS_PROJECT_ROOT}/.serial.log"
        WORKING_DIRECTORY "${UEFI_RUN_DIR}"
)