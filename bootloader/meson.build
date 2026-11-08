# UEFI applications use PE/COFF and the Microsoft x64 calling convention.
bootloader_lib = static_library(
    'bootloader',
    'src/main.c',
    dependencies: libuefi_dep,
    c_args: [
        '--target=x86_64-pc-windows-msvc',
        '-ffreestanding',
        '-fno-stack-protector',
        '-mno-red-zone',
    ],
    pic: false,
)

lld_link = find_program('lld-link')
bootloader_efi = custom_target(
    'bootloader-efi',
    input: bootloader_lib,
    output: 'bootloader.efi',
    command: [
        lld_link,
        '/subsystem:efi_application',
        '/entry:efi_main',
        '/nodefaultlib',
        '/machine:x64',
        '/out:@OUTPUT@',
        '@INPUT@',
    ],
    build_by_default: true,
)
