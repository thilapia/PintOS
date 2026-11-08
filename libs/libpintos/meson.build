libpintos_inc = include_directories('include')

libpintos_lib = static_library(
    'libpintos',
    ['src/sys/memory.c'],
    include_directories: libpintos_inc,
    c_args: freestanding_c_args,
    pic: false,
)

libpintos_dep = declare_dependency(
    link_with: libpintos_lib,
    include_directories: libpintos_inc,
)
