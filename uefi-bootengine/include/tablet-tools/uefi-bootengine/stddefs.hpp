#pragma once

extern "C" {
#include <efi.h>
#include <efilib.h>
}

using size_t = decltype(sizeof(0));
using nullptr_t = decltype(nullptr);