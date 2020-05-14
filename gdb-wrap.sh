#!/usr/bin/env bash

set -e

qemu_serial_file="$(realpath ./cmake-build-debug/uefi-bootengine/uefi-run/qemu-serial.txt)"
image_base="$( (grep "##DEBUG##" | tail -n 1 | awk '{printf "0x%x\n", strtonum($4)}') < "${qemu_serial_file}")"
if [[ -z "${image_base}" ]]; then
  echo "Failed to find image base address in qemu serial!"
  exit 1
fi

./calculate-gdb-file-load.sh "${image_base}"

