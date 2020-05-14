#pragma once

#include "tablet-tools/uefi-bootengine/std/type_traits.hpp"

namespace tablet_tools::uefi_bootengine {
    template <typename E, std::enable_if_t<std::is_integral_v<std::underlying_type_t<E>>, int> = 0>
    E operator |(E lhs, E rhs) {
        using underlying_e = std::underlying_type_t<E>;
        return static_cast<E>(static_cast<underlying_e>(lhs), static_cast<underlying_e>(rhs));
    }

    template <typename E, std::enable_if_t<std::is_integral_v<std::underlying_type_t<E>>, int> = 0>
    std::underlying_type_t<E> as_bitmask(E e) {
        return static_cast<std::underlying_type_t<E>>(e);
    }
}

#define AS_BITMASK(v) ::tablet_tools::uefi_bootengine::as_bitmask(v)
