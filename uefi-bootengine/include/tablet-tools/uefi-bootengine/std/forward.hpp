#pragma once

#include "tablet-tools/uefi-bootengine/std/type_traits.hpp"

namespace tablet_tools::uefi_bootengine::std {
    template <typename T>
    constexpr T &&forward(remove_reference_t<T> &t) noexcept {
        return static_cast<remove_reference_t<T>&&>(t);
    }

    template <typename T>
    constexpr T &&forward(remove_reference_t<T> &&t) noexcept {
        return static_cast<remove_reference_t<T>&&>(t);
    }
}