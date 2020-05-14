#pragma once

#include "tablet-tools/uefi-bootengine/stdlib.hpp"
#include "tablet-tools/uefi-bootengine/efi/simple_text_input_protocol.hpp"
#include "tablet-tools/uefi-bootengine/efi/protocol_handle.hpp"

namespace tablet_tools::uefi_bootengine {
    EFI_STATUS main();
    void key_read(efi_std::OpenedProtocolHandle<efi_std::SimpleTextInputProtocol> protocol);
}