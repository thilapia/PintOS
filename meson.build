project(
    'pintos',
    ['c', 'nasm'],
    version: '0.1.0',
    meson_version: '>=1.12.1',
    default_options: [
        'c_std=c23',
        'warning_level=3',
        'werror=true',
        'default_library=static',
        'b_lto=false',
        'b_pie=false',
        #'b_staticpie=false',
    ],
)

# toolchain

cc = meson.get_compiler('c')

# generic baremetal/freestanding C flags
freestanding_c_args = [
    '-ffreestanding',
    '-fno-builtin',
    '-fno-stack-protector',
    '-fno-pic',
    '-fno-pie',
    '-fno-plt',
    '-fno-asynchronous-unwind-tables',
    '-fno-unwind-tables',
    '-fno-omit-frame-pointer',
]

# kernel specific C flags
kernel_c_args = freestanding_c_args + [
    '-m64',
    '-mno-red-zone',
    '-mcmodel=kernel',
    '-mno-mmx',
    '-mno-sse',
    '-mno-sse2',
]

# kernel linker arguments
kernel_link_args = [
    '-nostdlib',
    '-static',
    '-no-pie',
    '-fuse-ld=lld',
    '-Wl,--build-id=none',
    '-Wl,-z,max-page-size=0x1000',
]

# subdirectories
subdir('libs')
