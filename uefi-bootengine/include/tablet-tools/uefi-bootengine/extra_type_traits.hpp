#pragma once

#include "tablet-tools/uefi-bootengine/stddefs.hpp"

namespace tablet_tools::uefi_bootengine {
    template <typename T, typename ...O>
    struct biggest_size {
        static constexpr size_t value = sizeof(T) > biggest_size<O...>::value ? sizeof(T) : biggest_size<O...>::value;
    };

    template <typename T>
    struct biggest_size<T> {
        static constexpr size_t value = sizeof(T);
    };

    template <typename T, typename ...O>
    constexpr size_t biggest_size_v = biggest_size<T, O...>::value;
}