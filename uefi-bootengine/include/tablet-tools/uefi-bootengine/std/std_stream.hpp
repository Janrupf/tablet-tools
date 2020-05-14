#pragma once

extern "C" {
#include <efi.h>
}
#include "type_traits.hpp"

#include "tablet-tools/uefi-bootengine/environment.hpp"

namespace tablet_tools::uefi_bootengine::std {
    class StdStream {
    private:
        SIMPLE_TEXT_OUTPUT_INTERFACE *interface;

        void change_interface(SIMPLE_TEXT_OUTPUT_INTERFACE *new_interface) {
            interface = new_interface;
        }

        friend EFI_STATUS Environment::run(EFI_HANDLE image_handle);
    public:
        explicit StdStream(SIMPLE_TEXT_OUTPUT_INTERFACE *interface);

        void write(const char16_t *data) const;
        void write(CHAR16 *data) const;
        void write(CHAR16 data) const;

        template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
        void write(const T &t) const {
            CHAR16 *formatted = PoolPrint(EFI_STR(u"%d"), t);
            write(formatted);
            Environment::uefi_call(gBS->FreePool, formatted);
        }

        template <typename T, std::enable_if_t<!std::is_same_v<char, T>, int> = 0>
        void write(const T *t) const {
            CHAR16 *formatted = PoolPrint(EFI_STR(u"0x%x"), t);
            write(formatted);
            Environment::uefi_call(gBS->FreePool, formatted);
        }

        template <typename T, std::enable_if_t<!std::is_invocable_v<T, StdStream &>, int> = 0>
        StdStream &operator <<(T t) {
            write(t);
            return *this;
        }

        template <typename T, std::enable_if_t<std::is_invocable_v<T, StdStream &>, int> = 0>
        StdStream &operator <<(T t) {
            t(*this);
            return *this;
        }
    };

    void endl(const StdStream &stream);

    extern StdStream cout;
    extern StdStream cerr;
}
