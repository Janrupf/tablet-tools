#!/usr/bin/env bash

if [[ -z "$1" ]]; then
  echo "Usage: $0 base-address"
  exit 1
fi
base_address=$1

efi_file="$(realpath ./cmake-build-debug/uefi-bootengine/uefi-bootenginex64.efi)"
pdb_file="$(realpath ./cmake-build-debug/uefi-bootengine/uefi-bootengine.pdb)"

sections=$(objdump --section-headers "${efi_file}" \
       | awk -F ' *' '$4 ~ /^[0-9a-f]+$/ {printf "%s:%s\n", $3, $5}')

gdb_command="add-symbol-file \"${pdb_file}\""
for section in $sections; do
  name="${section%%:*}"
  address=$(( 0x${section#*:} + base_address ))
  gdb_command="${gdb_command} -s ${name} ${address}"
done

echo "${gdb_command}"