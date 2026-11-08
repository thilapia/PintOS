# Compile the bootloader sources; a UEFI executable target will be added once
# the entry point and UEFI-specific linker setup are in place.
bootloader_lib = static_library(
    'bootloader',
    'src/main.c',
    dependencies: libuefi_dep,
    c_args: freestanding_c_args,
    pic: false,
)
