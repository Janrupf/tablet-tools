#pragma once

#include "tablet-tools/uefi-bootengine/std/std_stream.hpp"
#include "tablet-tools/uefi-bootengine/stddefs.hpp"

namespace tablet_tools::uefi_bootengine::std {
    class String {
    private:
        char16_t *native_string;

        static size_t length(const char16_t *str);

    public:
        explicit String();
        explicit String(const char16_t *str);
        explicit String(const CHAR16 *str);

        ~String();

        String(const String &other);
        String(String &&other) noexcept;

        String &operator=(const String &other);
        String &operator=(String &&other) noexcept;

        [[nodiscard]] CHAR16 *unsafe_efi_str() const;
        [[nodiscard]] const char16_t *c_str() const;
        [[nodiscard]] const CHAR16 *efi_str() const;
        CHAR16 *efi_str();

        [[nodiscard]] size_t length() const;
    };

    StdStream &operator<<(StdStream &stream, const String &str);
}