import gdb
import os.path as path
import pefile
import sys

script_path = path.dirname(path.realpath(__file__))
qemu_serial_log = path.join(script_path, ".serial.log")

print(f"[UEFI] Running in {script_path}")

if not path.exists(qemu_serial_log):
    print(f"[UEFI] .serial.log not found in {qemu_serial_log}")
    exit(1)

image_base_address = None

try:
    with open(qemu_serial_log, mode="r") as serial:
        for line in serial.readlines():
            if line.startswith("##DEBUG## Image base: "):
                image_base_address = int(line[22:-1], 16)

except IOError as e:
    print(f"[UEFI] Failed to open serial log: {e}", file=sys.stderr)
    exit(1)

if image_base_address is None:
    print(f"[UEFI] .serial.log did not include image base address", file=sys.stderr)
    exit(1)

efi_file = f"{script_path}/cmake-build-debug/uefi-bootengine/uefi-bootenginex64.efi"
debug_file = f"{script_path}/cmake-build-debug/uefi-bootengine/uefi-bootengine.pdb"

print(f"[UEFI] Image base address is 0x{image_base_address:x}")
gdb.execute("file", False, True)

sections = dict()

try:
    with open(efi_file, mode="rb") as opened_efi:
        for section in pefile.PE(data=opened_efi.read()).sections:
            sections[section.Name.decode("utf-8").strip("\x00")] = section.VirtualAddress

except IOError as e:
    print(f"[UEFI] Failed to open efi file {efi_file}: {e}")
    exit(1)

for name, address in sections.items():
    print(f"[UEFI]     Found section {name} at 0x{address:x}, relocating to 0x{image_base_address + address:x}")

gdb_command_params = " ".join(
    "-s " + name + " " + hex(address + image_base_address) for name, address in sections.items())
gdb_command = f"add-symbol-file {debug_file} {gdb_command_params}"
gdb.execute(gdb_command)

wait_symbol, _ = gdb.lookup_symbol("wait", gdb.newest_frame())
if wait_symbol is not None:
    wait_value = wait_symbol.value()
    if wait_value.address is not None:
        print(f"[UEFI] Found wait variable at 0x{wait_value.address:x}, setting to false...")
        gdb.selected_inferior().write_memory(wait_value.address, 1, 1)
    else:
        print("[UEFI] Failed to find address of wait variable", file=sys.stderr)
else:
    print("[UEFI] Failed to lookup wait variable")