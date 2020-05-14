#pragma once

#include "tablet-tools/uefi-bootengine/std/type_traits.hpp"
#include "tablet-tools/uefi-bootengine/efi/efi_type_traits.hpp"
#include "tablet-tools/uefi-bootengine/stddefs.hpp"

extern "C" {
#include <efi.h>
#include <efilib.h>
}

#define EFI_STR(x) reinterpret_cast<CHAR16 *>(const_cast<char16_t *>(x))

extern "C" void efi_exception_callback(EFI_EXCEPTION_TYPE exception_type, EFI_SYSTEM_CONTEXT context);

namespace tablet_tools::uefi_bootengine {
    namespace std {
        class String;
    }

    class Environment {
    private:
        template <typename T, std::enable_if_t<!std::is_same_v<T, nullptr_t>, int> = 0>
        static UINT64 cast_to_uint64(T t) {
            return static_cast<UINT64>(t);
        }

        template <typename T, std::enable_if_t<std::is_same_v<T, nullptr_t>, int> = 0>
        static UINT64 cast_to_uint64(T) {
            return 0;
        }

        template <typename T>
        static UINT64 cast_to_uint64(T *t) {
            return reinterpret_cast<UINT64>(t);
        }

    public:
        static EFI_HANDLE image_handle;

        static EFI_STATUS run(EFI_HANDLE image_handle);

        static void *malloc(size_t size);
        static void free(void *data);

        static std::String status_to_string(EFI_STATUS status);

        template <typename Fn, typename ...Args,
                typename ReturnType = std::invoke_result_t<Fn,
                efi_std::decay_to_efi_t<std::remove_cv_t<std::remove_reference_t<Args>>>...>>
        static ReturnType uefi_call(Fn &&fn, Args &&...args) {
            static_assert(sizeof...(Args) <= 10,
                    "sizeof...(Args) is greater than 10, this is not allowed for UEFI calls");

            if constexpr(sizeof...(Args) == 0) {
                return reinterpret_cast<ReturnType>(efi_call0(reinterpret_cast<void *>(fn)));
            } else if constexpr(sizeof...(Args) == 1) {
                return reinterpret_cast<ReturnType>(efi_call1(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            } else if constexpr(sizeof...(Args) == 2) {
                return reinterpret_cast<ReturnType>(efi_call2(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            } else if constexpr(sizeof...(Args) == 3) {
                return reinterpret_cast<ReturnType>(efi_call3(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            } else if constexpr(sizeof...(Args) == 4) {
                return reinterpret_cast<ReturnType>(efi_call4(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            } else if constexpr(sizeof...(Args) == 5) {
                return reinterpret_cast<ReturnType>(efi_call5(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            } else if constexpr(sizeof...(Args) == 6) {
                return reinterpret_cast<ReturnType>(efi_call6(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            } else if constexpr(sizeof...(Args) == 7) {
                return reinterpret_cast<ReturnType>(efi_call7(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            } else if constexpr(sizeof...(Args) == 8) {
                return reinterpret_cast<ReturnType>(efi_call8(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            } else if constexpr(sizeof...(Args) == 9) {
                return reinterpret_cast<ReturnType>(efi_call1(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            } else if constexpr(sizeof...(Args) == 10) {
                return reinterpret_cast<ReturnType>(efi_call10(
                        reinterpret_cast<void *>(fn),
                        cast_to_uint64(args)...)
                );
            }
        }
    };
}