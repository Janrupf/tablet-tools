#pragma once

#include "tablet-tools/uefi-bootengine/environment.hpp"
#include "tablet-tools/uefi-bootengine/std/forward.hpp"
#include "tablet-tools/uefi-bootengine/std/function.hpp"
#include "tablet-tools/uefi-bootengine/std/move.hpp"
#include "tablet-tools/uefi-bootengine/std/shared_ptr.hpp"
#include "tablet-tools/uefi-bootengine/std/std_stream.hpp"
#include "tablet-tools/uefi-bootengine/std/type_traits.hpp"
#include "tablet-tools/uefi-bootengine/std/vector.hpp"

void *malloc(size_t size);
void free(void *data);

void *reallocate(void *src, size_t src_size, size_t new_size);

template <typename T, typename X = T>
void memcpy(T *dest, const X *src, size_t bytes) {
    const char *c_src = reinterpret_cast<const char *>(src);
    char *c_dest = reinterpret_cast<char *>(dest);
    for(size_t i = 0; i < bytes; i++) {
        c_dest[i] = c_src[i];
    }
}

template <typename T>
void objcpy(T *dest, const T *src, size_t count) {
    memcpy<T>(dest, src, sizeof(T) * count);
}

template <typename T>
void objcpy(T *dest, const T *src) {
    memcpy<T>(dest, src, sizeof(T));
}

template <typename T, typename X, tablet_tools::uefi_bootengine::std::enable_if_t<sizeof(X) == 1, int> = 0>
void memset(T *dest, X value, size_t count) {
    char *c_dest = reinterpret_cast<char *>(dest);
    for(size_t i = 0; i < count; i++) {
        c_dest[i] = static_cast<char>(value);
    }
}

void *operator new(size_t size);
void *operator new[](size_t size);

template <typename T>
void *operator new(size_t, T *t) {
    return static_cast<void *>(t);
}

void operator delete(void *p);
void operator delete(void *p, size_t size);
void operator delete[](void *p);

extern "C" void __cxa_pure_virtual();