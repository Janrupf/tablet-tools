#include "tablet-tools/uefi-bootengine/environment.hpp"

extern "C" EFI_STATUS EFIAPI efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    InitializeLib(image_handle, system_table);

    EFI_LOADED_IMAGE *loaded_image;
    EFI_STATUS get_image_status = efi_call3(
            reinterpret_cast<void *>(system_table->BootServices->HandleProtocol),
            (UINT64) image_handle,
            reinterpret_cast<UINT64>(&LoadedImageProtocol),
            reinterpret_cast<UINT64>(&loaded_image)
            );
    if(EFI_ERROR(get_image_status)) {
        Print(reinterpret_cast<const CHAR16 *>(u"Failed to get image base address: %r!\n"), get_image_status);
    } else {
        Print(reinterpret_cast<const CHAR16 *>(u"##DEBUG## Image base: 0x%lx\n"), loaded_image->ImageBase);
        // bool wait = true;
        // while(wait) {
        //     __asm__ __volatile__("pause");
        // }
    }

    return tablet_tools::uefi_bootengine::Environment::run(image_handle);
}