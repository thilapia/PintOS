#include <uefi/uefi.h>

void print_hello(efi_simple_text_output_protocol_t* console);
efi_status_t wait_for_key(
  efi_boot_services_t* boot_services,
  efi_simple_text_output_protocol_t* console,
  efi_simple_text_input_protocol_t* input
);

efi_status_t EFIAPI
efi_main(efi_handle_t image_handle, efi_system_table_t* system_table) {
  (void)image_handle;

  efi_boot_services_t* boot_services = system_table->boot_services;
  efi_simple_text_output_protocol_t* console = system_table->con_out;
  efi_simple_text_input_protocol_t* input = system_table->con_in;

  efi_status_t status = console->clear_screen(console);
  if (status != EFI_SUCCESS) {
    return status;
  }

  print_hello(console);
  return wait_for_key(boot_services, console, input);
}

void print_hello(efi_simple_text_output_protocol_t* console) {
  console->output_string(console, u"PintOS Bootloader\r\n");
}

efi_status_t wait_for_key(
  efi_boot_services_t* boot_services,
  efi_simple_text_output_protocol_t* console,
  efi_simple_text_input_protocol_t* input
) {
  console->output_string(console, u"Press any key to continue...");

  efi_event_t key_stroke_event = input->wait_for_key;
  uintptr_t event_index = 0;

  efi_status_t status =
    boot_services->wait_for_event(1, &key_stroke_event, &event_index);

  if (status != EFI_SUCCESS) {
    return status;
  }

  efi_input_key_t key;
  status = input->read_key_stroke(input, &key);
  if (status != EFI_SUCCESS) {
    return status;
  }

  return EFI_SUCCESS;
}
