#pragma once

#include <stdint.h>

// type definitions
typedef uintptr_t efi_status_t;
typedef void* efi_handle_t;
typedef void* efi_event_t;

// constants
#define EFI_SUCCESS ((efi_status_t)0)

#ifdef __x86_64__
#define EFIAPI __attribute__((ms_abi))
#else
#error "The UEFI calling convention is not defined for this architecture"
#endif

// EFI System Table
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

// EFI Boot Services
typedef void(EFIAPI* efi_unused_service_t)(void);

typedef efi_status_t(EFIAPI* efi_wait_for_event_t)(
  uintptr_t number_of_events, efi_event_t* events, uintptr_t* index
);

struct efi_boot_service {
  efi_table_header_t header;
  efi_unused_service_t raise_tpl;
  efi_unused_service_t restore_tpl;
  efi_unused_service_t allocate_pages;
  efi_unused_service_t free_pages;
  efi_unused_service_t get_memory_map;
  efi_unused_service_t allocate_pool;
  efi_unused_service_t free_pool;
  efi_unused_service_t create_event;
  efi_unused_service_t set_timer;
  efi_wait_for_event_t wait_for_event;
};

// Simple Text Output Protocol
typedef efi_status_t(EFIAPI* efi_text_clear_screen_t)(
  efi_simple_text_output_protocol_t* self
);

typedef efi_status_t(EFIAPI* efi_text_reset_t)(
  efi_simple_text_output_protocol_t* self, uint8_t extended_verification
);

typedef efi_status_t(EFIAPI* efi_text_string_t)(
  efi_simple_text_output_protocol_t* self, uint16_t* string
);

struct efi_simple_text_output_protocol {
  efi_text_reset_t reset;
  efi_text_string_t output_string;
  efi_unused_service_t test_string;
  efi_unused_service_t query_mode;
  efi_unused_service_t set_mode;
  efi_unused_service_t set_attribute;
  efi_text_clear_screen_t clear_screen;
};

// Simple Text Input Protocol
typedef struct {
  uint16_t scan_code;
  uint16_t unicode_char;
} efi_input_key_t;

typedef efi_status_t(EFIAPI* efi_input_reset_t)(
  efi_simple_text_input_protocol_t* self, uint8_t extended_verification
);

typedef efi_status_t(EFIAPI* efi_input_read_key_t)(
  efi_simple_text_input_protocol_t* self, efi_input_key_t* key
);

struct efi_simple_text_input_protocol {
  efi_input_reset_t reset;
  efi_input_read_key_t read_key_stroke;
  efi_event_t wait_for_key;
};
