#include <bootloader/elf.h>

static uint16_t read_u16(const uint8_t* data) {
  return (uint16_t)((uint32_t)data[0] | ((uint32_t)data[1] << 8U));
}

static uint32_t read_u32(const uint8_t* data) {
  return (uint32_t)data[0] | ((uint32_t)data[1] << 8U) |
         ((uint32_t)data[2] << 16U) | ((uint32_t)data[3] << 24U);
}

bool elf64_validate_header(const uint8_t* data, size_t size) {
  if (data == NULL || size < sizeof(elf64_header_t)) {
    return false;
  }

  return (data[0] == 0x7f && data[1] == 'E' && data[2] == 'L' &&
          data[3] == 'F' && data[EI_CLASS] == ELFCLASS64 &&
          data[EI_DATA] == ELFDATA2LSB && data[EI_VERSION] == EV_CURRENT &&
          read_u16(data + offsetof(elf64_header_t, type)) == ET_EXEC &&
          read_u16(data + offsetof(elf64_header_t, machine)) == EM_X86_64 &&
          read_u32(data + offsetof(elf64_header_t, version)) == EV_CURRENT &&
          read_u16(data + offsetof(elf64_header_t, ehsize)) ==
            sizeof(elf64_header_t)) != 0;
}
