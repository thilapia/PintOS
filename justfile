# List available commands.
default:
    @just --list

# Configure a cross build in a new build directory.
setup build_dir="build":
    meson setup '{{build_dir}}' --cross-file cross/x86_64.ini

# Compile an already configured build.
build build_dir="build":
    meson compile -C '{{build_dir}}'

# Run the Meson test suite.
test build_dir="build":
    meson test -C '{{build_dir}}' --print-errorlogs

# Format C sources and headers.
format:
    git ls-files -z --cached --others --exclude-standard -- '*.c' '*.h' | xargs -0 -r clang-format -i

# Check formatting of C sources and headers.
check-format:
    git ls-files -z --cached --others --exclude-standard -- '*.c' '*.h' | xargs -0 -r clang-format --dry-run --Werror

# Run clang-tidy using the build's compilation database.
clang-tidy build_dir="build":
    git ls-files -z --cached --others --exclude-standard -- '*.c' | xargs -0 -r clang-tidy -p '{{build_dir}}' --quiet

# Run both style checks.
check-style build_dir="build": (check-format) (clang-tidy build_dir)

# Build the project and package the UEFI executable as a qcow2 disk.
image build_dir="build": (build build_dir)
    python3 scripts/mkimage.py '{{build_dir}}/bootloader/bootloader.efi' '{{build_dir}}/pintos.qcow2' --kernel '{{build_dir}}/kernel/kernel.elf'

# Build an image, then boot it with OVMF. Firmware variables persist in build_dir.
run build_dir="build": (image build_dir)
    test -f '{{build_dir}}/OVMF_VARS.fd' || cp /usr/share/edk2/x64/OVMF_VARS.4m.fd '{{build_dir}}/OVMF_VARS.fd'
    qemu-system-x86_64 -machine q35 -m 256M -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/x64/OVMF_CODE.4m.fd -drive if=pflash,format=raw,file='{{build_dir}}/OVMF_VARS.fd' -drive if=virtio,format=qcow2,file='{{build_dir}}/pintos.qcow2'
