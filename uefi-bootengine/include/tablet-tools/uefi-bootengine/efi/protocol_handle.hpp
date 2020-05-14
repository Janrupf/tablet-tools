#pragma once

#include "tablet-tools/uefi-bootengine/stddefs.hpp"
#include "tablet-tools/uefi-bootengine/result.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    template<typename T>
    class OpenedProtocolHandle {
    public:
        using ProtocolType = typename T::ProtocolType;

    private:
        EFI_HANDLE handle;
        T protocol;
        size_t *use_count;
        bool exclusive;

        void close() {
            if(exclusive) {
                Environment::uefi_call(
                        gBS->CloseProtocol,
                        handle,
                        T::get_guid(),
                        Environment::image_handle,
                        nullptr
                );
            }
            delete use_count;
        }

    public:
        explicit OpenedProtocolHandle(EFI_HANDLE handle, ProtocolType *interface, bool exclusive)
                : handle(handle), protocol(interface), exclusive(exclusive), use_count(new size_t(1)) {}

        OpenedProtocolHandle(const OpenedProtocolHandle &other) : handle(other.handle), protocol(other.protocol),
            use_count(other.use_count) {
            (*use_count)++;
        }

        OpenedProtocolHandle(OpenedProtocolHandle &&other) noexcept : handle(other.handle), protocol(other.protocol),
            use_count(other.use_count) {
            other.use_count = nullptr;
            other.handle = nullptr;
        }

        OpenedProtocolHandle &operator=(const OpenedProtocolHandle &other) {
            if(&other != this && use_count != nullptr) {
                *(use_count)--;
                if(*use_count == 0) {
                    close();
                }
                handle = other.handle;
                protocol = other.protocol;
                use_count = other.use_count;
                *(use_count)++;
            }
            return *this;
        }

        OpenedProtocolHandle &operator=(OpenedProtocolHandle &&other) noexcept {
            if(&other != this && use_count != nullptr) {
                *(use_count)--;
                if(*use_count == 0) {
                    close();
                }
                handle = other.handle;
                protocol = other.protocol;
                use_count = other.use_count;

                other.use_count = nullptr;
                other.handle = nullptr;
            }
            return *this;
        }

        ~OpenedProtocolHandle() {
            if(use_count != nullptr && --(*use_count) == 0) {
                close();
            }
        }

        T *operator ->() {
            return &protocol;
        }

        const T *operator ->() const {
            return &protocol;
        }
    };

    template<typename T>
    class ProtocolHandle {
    public:
        using ProtocolType = typename T::ProtocolType;

    private:
        EFI_HANDLE handle;

    public:
        explicit ProtocolHandle(EFI_HANDLE handle) : handle(handle) {}

        Result<OpenedProtocolHandle<T>, EFI_STATUS> open_exclusive() const {
            ProtocolType *interface;
            EFI_STATUS status = Environment::uefi_call(
                    gBS->OpenProtocol,
                    handle,
                    T::get_guid(),
                    reinterpret_cast<void **>(&interface),
                    Environment::image_handle,
                    nullptr,
                    EFI_OPEN_PROTOCOL_EXCLUSIVE
            );
            if(status != EFI_SUCCESS) {
                return Result<OpenedProtocolHandle<T>, EFI_STATUS>::make_error(status);
            }

            return Result<OpenedProtocolHandle<T>, EFI_STATUS>::make_ok(
                    OpenedProtocolHandle<T>(handle, interface, true));
        }

        Result<OpenedProtocolHandle<T>, EFI_STATUS> open() const {
            ProtocolType *interface;
            EFI_STATUS status = Environment::uefi_call(
                    gBS->OpenProtocol,
                    handle,
                    T::get_guid(),
                    reinterpret_cast<void **>(&interface),
                    Environment::image_handle,
                    nullptr,
                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
                    );
            if(status != EFI_SUCCESS) {
                return Result<OpenedProtocolHandle<T>, EFI_STATUS>::make_error(status);
            }

            return Result<OpenedProtocolHandle<T>, EFI_STATUS>::make_ok(
                    OpenedProtocolHandle<T>(handle, interface, false));
        }
    };
}