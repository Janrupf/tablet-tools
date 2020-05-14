#pragma once

#include "tablet-tools/uefi-bootengine/std/type_traits.hpp"

namespace tablet_tools::uefi_bootengine::std {
    template <typename T>
    constexpr remove_reference_t<T> &&move(T &&arg) noexcept {
        return static_cast<remove_reference_t<T>&&>(arg);
    }
}