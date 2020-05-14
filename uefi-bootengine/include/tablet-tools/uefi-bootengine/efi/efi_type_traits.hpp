#pragma once

extern "C" {
#include <efilib.h>
}

#define DECAY(orig, target) \
    template <> \
    struct decay_to_efi<orig> { \
        using type = target; \
    }; \
    \
    template <> \
    struct decay_to_efi<orig *> { \
        using type = target *; \
    }; \
    \
    template <> \
    struct decay_to_efi<const orig *> { \
        using type = target *; \
    };

namespace tablet_tools::uefi_bootengine::efi_std {
    template <typename T>
    struct decay_to_efi {
        using type = T;
    };

    DECAY(char16_t, CHAR16);

    template <typename T>
    using decay_to_efi_t = typename decay_to_efi<T>::type;
}

#undef DECAY