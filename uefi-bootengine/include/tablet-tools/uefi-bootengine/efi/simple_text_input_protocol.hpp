#pragma once

#include "tablet-tools/uefi-bootengine/result.hpp"
#include "tablet-tools/uefi-bootengine/stddefs.hpp"

namespace tablet_tools::uefi_bootengine::efi_std {
    class SimpleTextInputProtocol {
    public:
        using ProtocolType = EFI_SIMPLE_TEXT_IN_PROTOCOL;

    private:
        EFI_SIMPLE_TEXT_IN_PROTOCOL *interface;

    public:
        static EFI_GUID *get_guid();

        explicit SimpleTextInputProtocol(EFI_SIMPLE_TEXT_IN_PROTOCOL *interface);
        EFI_STATUS reset(bool extended_verification = false);
        Result<EFI_INPUT_KEY, EFI_STATUS> read_key_stroke();
        [[nodiscard]] EFI_EVENT wait_for_key_event() const;
    };
}