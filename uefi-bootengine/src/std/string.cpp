#include "tablet-tools/uefi-bootengine/std/string.hpp"

#include "tablet-tools/uefi-bootengine/stdlib.hpp"

namespace tablet_tools::uefi_bootengine::std {
    String::String() : native_string(static_cast<char16_t *>(malloc(sizeof(char16_t)))) {
        native_string[0] = u'\0';
    }

    String::String(const char16_t *str)
        : native_string(static_cast<char16_t *>(malloc(sizeof(char16_t) * (length(str) + 1)))) {
        objcpy(native_string, str, length(str));
        native_string[length(str)] = u'\0';
    }

    String::String(const CHAR16 *str)
        : native_string(static_cast<char16_t *>(malloc(sizeof(char16_t) *
            (length(reinterpret_cast<const char16_t *>(str)) + 1)))) {
        memcpy(native_string, str, length(reinterpret_cast<const char16_t *>(str)) * sizeof(char16_t));
        native_string[length(reinterpret_cast<const char16_t *>(str))] = u'\0';
    }

    String::~String() {
        free(native_string);
    }

    String::String(const String &other)
            : native_string(static_cast<char16_t *>(malloc(sizeof(char16_t) * (other.length() + 1)))) {
        objcpy(native_string, other.native_string, other.length() + 1);
    }

    String::String(String &&other) noexcept
            : native_string(static_cast<char16_t *>(malloc(sizeof(char16_t) * (other.length() + 1)))) {
        objcpy(native_string, other.native_string, other.length() + 1);
        free(other.native_string);
        other.native_string = static_cast<char16_t *>(malloc(sizeof(char16_t)));
        *other.native_string = u'\0';
    }

    String &String::operator=(const String &other) {
        if(this != &other) {
            free(native_string);
            native_string = static_cast<char16_t *>(malloc(sizeof(char16_t) * (other.length() + 1)));
            objcpy(native_string, other.native_string, other.length() + 1);
        }
        return *this;
    }

    String &String::operator=(String &&other) noexcept {
        free(native_string);
        native_string = static_cast<char16_t *>(malloc(sizeof(char16_t) * (other.length() + 1)));
        objcpy(native_string, other.native_string, other.length() + 1);

        free(other.native_string);
        other.native_string = static_cast<char16_t *>(malloc(sizeof(char16_t)));
        *other.native_string = u'\0';
        return *this;
    }

    const char16_t *String::c_str() const {
        return native_string;
    }

    const CHAR16 *String::efi_str() const {
        return reinterpret_cast<const CHAR16 *>(native_string);
    }

    CHAR16 *String::efi_str() {
        return reinterpret_cast<CHAR16 *>(native_string);
    }

    size_t String::length() const {
        return length(native_string);
    }

    CHAR16 *String::unsafe_efi_str() const {
        return reinterpret_cast<CHAR16 *>(const_cast<char16_t *>(native_string));
    }

    size_t String::length(const char16_t *str) {
        size_t i = 0;
        while(*(str++) != u'\0') {
            i++;
        }
        return i;
    }

    StdStream &operator<<(StdStream &stream, const String &str) {
        stream << str.c_str();
        return stream;
    }
}