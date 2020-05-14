#pragma once

#include "tablet-tools/uefi-bootengine/efi/protocol_handle.hpp"
#include "tablet-tools/uefi-bootengine/std/vector.hpp"
#include "tablet-tools/uefi-bootengine/result.hpp"

#include "tablet-tools/uefi-bootengine/std/string.hpp"
#include "tablet-tools/uefi-bootengine/efi/protocol/efi_usb.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    template <typename T>
    Result<std::Vector<ProtocolHandle<T>>, EFI_STATUS> find_protocols() {
        UINTN buffer_size = 0;

        EFI_STATUS status = Environment::uefi_call(
                gBS->LocateHandle,
                ByProtocol,
                T::get_guid(),
                nullptr,
                &buffer_size,
                nullptr
                );
        if(status != EFI_SUCCESS && status != EFI_BUFFER_TOO_SMALL) {
            std::cout << u"LOCATE_HANDLE: " << Environment::status_to_string(status) << std::endl;
            return Result<std::Vector<ProtocolHandle<T>>, EFI_STATUS>::make_error(status);
        }

        std::Vector<EFI_HANDLE> buffer(buffer_size / sizeof(EFI_HANDLE));
        status = Environment::uefi_call(
                gBS->LocateHandle,
                ByProtocol,
                T::get_guid(),
                nullptr,
                &buffer_size,
                buffer.data()
                );
        if(status != EFI_SUCCESS) {
            return Result<std::Vector<ProtocolHandle<T>>, EFI_STATUS>::make_error(status);
        }

        return Result<std::Vector<ProtocolHandle<T>>, EFI_STATUS>::make_ok(buffer.map<ProtocolHandle<T>>());
    }
}