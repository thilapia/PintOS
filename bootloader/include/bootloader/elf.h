#pragma once

#include <stdint.h>

#define EI_NIDENT 16

// Object file types and machine architectures.
#define ET_EXEC 2
#define EM_X86_64 62

typedef uint16_t elf64_half_t;
typedef uint32_t elf64_word_t;
typedef uint64_t elf64_addr_t;
typedef uint64_t elf64_off_t;

typedef struct {
  uint8_t ident[EI_NIDENT];
  elf64_half_t type;
  elf64_half_t machine;
  elf64_word_t version;
  elf64_addr_t entry_address;
  elf64_off_t program_header_offset;
  elf64_off_t section_header_offset;
  elf64_word_t flags;
  elf64_half_t ehsize;
  elf64_half_t program_header_ent_size;
  elf64_half_t program_header_num;
  elf64_half_t section_header_ent_size;
  elf64_half_t section_header_num;
  elf64_half_t section_header_str_ndx;
} elf64_header_t;

_Static_assert(sizeof(elf64_header_t) == 64, "ELF64 header must be 64 bytes");
