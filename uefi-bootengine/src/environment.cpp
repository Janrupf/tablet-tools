#include "tablet-tools/uefi-bootengine/environment.hpp"
#include "tablet-tools/uefi-bootengine/application/application.hpp"

#include "tablet-tools/uefi-bootengine/std/string.hpp"

extern "C" void efi_exception_callback(EFI_EXCEPTION_TYPE exception_type, EFI_SYSTEM_CONTEXT generic_context) {
    namespace std = tablet_tools::uefi_bootengine::std;

    switch(exception_type) {
        case EXCEPT_X64_PAGE_FAULT: {
            std::cerr << u"!!!PAGE FAULT!!!" << std::endl;
#if __x86_64__
            EFI_SYSTEM_CONTEXT_X64 *context = generic_context.SystemContextX64;

            std::cerr << u"    Fault at: " << context->Rip << std::endl;
#else
            std::cerr << u"Exception information not implemented on non x86_64..." << std::endl;
#endif
            break;
        }

        default: {
            std::cerr << u"Unhandled exception type: " << exception_type << std::endl;
            break;
        }
    }

    std::cerr << u"Halting system!" << std::endl;
    while(true) {
        __asm__ __volatile__ ("hlt");
    }
}

namespace tablet_tools::uefi_bootengine {
    EFI_HANDLE Environment::image_handle;

    EFI_STATUS Environment::run(EFI_HANDLE new_image_handle) {
        Environment::image_handle = new_image_handle;
        std::cout.change_interface(gST->ConOut);
        std::cerr.change_interface(gST->ConOut); // StdErr does not show up
        // std::cerr.change_interface(gST->StdErr);

        EFI_DEBUG_SUPPORT_PROTOCOL *debug_protocol = nullptr;

        UINTN handle_count;
        EFI_HANDLE *handles;
        EFI_STATUS locate_handle_status = uefi_call(
                gBS->LocateHandleBuffer,
                ByProtocol,
                &gEfiDebugSupportProtocolGuid,
                nullptr,
                &handle_count,
                &handles
        );

        switch(locate_handle_status) {
            case EFI_SUCCESS: {
                for(size_t i = 0; i < handle_count; i++) {
                    EFI_STATUS open_protocol_status = uefi_call(
                            gBS->OpenProtocol,
                            handles[i],
                            &gEfiDebugSupportProtocolGuid,
                            reinterpret_cast<void **>(&debug_protocol),
                            image_handle,
                            nullptr,
                            EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
                    if(EFI_ERROR(open_protocol_status)) {
                        std::cerr << u"Failed to open debug protocol: " << open_protocol_status << std::endl;
                        continue;
                    }

                    if(debug_protocol->Isa == IsaX64) {
                        break;
                    }

                    EFI_STATUS close_protocol_status = uefi_call(
                            gBS->CloseProtocol,
                            handles[i],
                            &gEfiDebugSupportProtocolGuid,
                            image_handle,
                            nullptr
                    );

                    if(EFI_ERROR(close_protocol_status)) {
                        std::cerr << u"Failed to close debug protocol: " << close_protocol_status << std::endl;
                    }

                    debug_protocol = nullptr;
                }
                break;
            }

            case EFI_NOT_FOUND: {
                std::cout << u"Efi debug protocol not found, exception handling not available!" << std::endl;
                break;
            }

            default: {
                std::cout << u"Unexpected error while locating debug protocol: " << locate_handle_status << std::endl;
                break;
            }
        }

        uefi_call(gBS->FreePool, handles);

        if(!debug_protocol) {
            std::cerr << u"Failed to locate EFI debug protocol for x86_64, exception handling not available!"
                      << std::endl;
        } else {
            EFI_STATUS callback_register_status = uefi_call(
                    debug_protocol->RegisterExceptionCallback,
                    debug_protocol,
                    0,
                    efi_exception_callback,
                    EXCEPT_X64_PAGE_FAULT
            );
            if(EFI_ERROR(callback_register_status)) {
                std::cerr << u"Failed to register exception callback" << std::endl;
            }
        }

        return main();
    }

    void *Environment::malloc(size_t size) {
        void *data;
        EFI_STATUS allocation_status = uefi_call(
                gBS->AllocatePool,
                EfiBootServicesData,
                size,
                &data
                );
        if(EFI_ERROR(allocation_status)) {
            std::cerr << u"Allocation error!" << std::endl;
            return nullptr;
        }
        return data;
    }

    void Environment::free(void *data) {
        if(data) {
            uefi_call(gBS->FreePool, data);
        }
    }

    std::String Environment::status_to_string(EFI_STATUS status) {
        CHAR16 *pool = PoolPrint(reinterpret_cast<CHAR16 *>(const_cast<char16_t *>(u"%r")), status);
        std::String error_message(pool);
        uefi_call(gBS->FreePool, pool);
        return error_message;
    }
}