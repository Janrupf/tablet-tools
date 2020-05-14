#include "tablet-tools/uefi-bootengine/stdlib.hpp"

void *malloc(size_t size) {
    return tablet_tools::uefi_bootengine::Environment::malloc(size);
}

void free(void *data) {
    tablet_tools::uefi_bootengine::Environment::free(data);
}

void *reallocate(void *src, size_t src_size, size_t new_size) {
    void *new_data = malloc(new_size);
    memcpy(new_data, src, src_size);
    free(src);
    return new_data;
}

void *operator new(size_t size) {
    return malloc(size);
}

void *operator new[](size_t size) {
    void *val = malloc(size);
    memset(val, '\0', size);
    return val;
}

void operator delete(void *p) {
    free(p);
}

void operator delete(void *p, size_t) {
    free(p);
}

void operator delete[](void *p) {
    free(p);
}

extern "C" void __cxa_pure_virtual() {
    Print(reinterpret_cast<CHAR16 *>(const_cast<char16_t *>(u"PURE VIRTUAL FUNCTION CALLED!")));
}