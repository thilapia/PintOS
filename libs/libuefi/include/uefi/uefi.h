#pragma once

#include <stdint.h>

// type definitions
typedef uintptr_t efi_status_t;
typedef void* efi_handle_t;

// constants
#define EFI_SUCCESS ((efi_status_t)0)

#ifdef __x86_64__
#define EFIAPI __attribute__((ms_abi))
#else
#error "The UEFI calling convention is not defined for this architecture"
#endif

// table definitions
typedef struct {
  uint64_t signature;
  uint32_t revision;
  uint32_t header_size;
  uint32_t crc32;
  uint32_t reserved;
} efi_table_header_t;

typedef struct efi_simple_text_input_protocol efi_simple_text_input_protocol_t;
typedef struct efi_simple_text_output_protocol
  efi_simple_text_output_protocol_t;
typedef struct efi_runtime_services efi_runtime_services_t;
typedef struct efi_boot_service efi_boot_services_t;
typedef struct efi_configuration_table efi_configuration_table_t;

typedef struct efi_system_table {
  efi_table_header_t header;
  uint16_t* firmware_vendor;
  uint32_t firmware_revision;
  efi_handle_t console_in_handle;
  efi_simple_text_input_protocol_t* con_in;
  efi_handle_t console_out_handle;
  efi_simple_text_output_protocol_t* con_out;
  efi_handle_t standard_error_handle;
  efi_simple_text_output_protocol_t* std_err;
  efi_runtime_services_t* runtime_services;
  efi_boot_services_t* boot_services;
  uintptr_t number_of_table_entries;
  efi_configuration_table_t* configuration_table;
} efi_system_table_t;
