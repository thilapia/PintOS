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
