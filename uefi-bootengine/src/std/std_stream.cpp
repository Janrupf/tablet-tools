#include "tablet-tools/uefi-bootengine/std/std_stream.hpp"

extern "C" {
#include <efilib.h>
}

namespace tablet_tools::uefi_bootengine::std {
    StdStream::StdStream(SIMPLE_TEXT_OUTPUT_INTERFACE *interface) : interface(interface) {}

    void StdStream::write(const char16_t *data) const {
        Environment::uefi_call(interface->OutputString, interface, data);
    }

    void StdStream::write(CHAR16 *data) const {
        Environment::uefi_call(interface->OutputString, interface, data);
    }

    void StdStream::write(CHAR16 data) const {
        CHAR16 buffer[2];
        buffer[0] = data;
        buffer[1] = u'\0';
        Environment::uefi_call(interface->OutputString, interface, buffer);
    }

    void endl(const StdStream &stream) {
        stream.write(u"\r\n");
    }

    StdStream cout(gST->ConOut);
    StdStream cerr(gST->StdErr);
}
