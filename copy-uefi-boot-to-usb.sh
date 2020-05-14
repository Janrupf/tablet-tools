#!/usr/bin/env bash

sudo mount /dev/sde1 /mnt/usb
sudo mkdir -p /mnt/usb/EFI/BOOT
sudo cp cmake-build-debug/uefi-bootengine/uefi-bootenginex64.efi /mnt/usb/EFI/BOOT/BOOTx64.EFI
sudo umount /dev/sde1