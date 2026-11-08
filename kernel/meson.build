kernel = executable(
    'kernel.elf',
    'src/main.c',
    c_args: kernel_c_args,
    link_args: kernel_link_args + ['-Wl,-e,kernel_main'],
    install: false,
)
