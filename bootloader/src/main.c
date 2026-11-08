#include <bootloader/elf.h>
#include <uefi/uefi.h>

static efi_status_t check_kernel(
  efi_handle_t image,
  efi_boot_services_t* boot_services,
  efi_simple_text_output_protocol_t* console
);

void print_hello(efi_simple_text_output_protocol_t* console);
efi_status_t wait_for_key(
  efi_boot_services_t* boot_services,
  efi_simple_text_output_protocol_t* console,
  efi_simple_text_input_protocol_t* input
);

efi_status_t EFIAPI
efi_main(efi_handle_t image_handle, efi_system_table_t* system_table) {
  efi_boot_services_t* boot_services = system_table->boot_services;
  efi_simple_text_output_protocol_t* console = system_table->con_out;
  efi_simple_text_input_protocol_t* input = system_table->con_in;

  efi_status_t status = console->clear_screen(console);
  if (status != EFI_SUCCESS) {
    return status;
  }

  print_hello(console);
  status = check_kernel(image_handle, boot_services, console);
  if (status != EFI_SUCCESS) {
    return status;
  }
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

static efi_status_t check_kernel(
  efi_handle_t image,
  efi_boot_services_t* boot_services,
  efi_simple_text_output_protocol_t* console
) {
  efi_guid_t loaded_image_guid = {
    0x5b1b31a1,
    0x9562,
    0x11d2,
    {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b},
  };
  efi_guid_t file_system_guid = {
    0x964e5b22,
    0x6459,
    0x11d2,
    {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b},
  };
  void* interface = NULL;
  efi_status_t status =
    boot_services->handle_protocol(image, &loaded_image_guid, &interface);
  if (status != EFI_SUCCESS) {
    console->output_string(
      console, u"Cannot access loaded image protocol.\r\n"
    );
    return status;
  }
  efi_loaded_image_t* loaded_image = interface;
  interface = NULL;
  status = boot_services->handle_protocol(
    loaded_image->device_handle, &file_system_guid, &interface
  );
  if (status != EFI_SUCCESS) {
    console->output_string(console, u"Cannot access boot filesystem.\r\n");
    return status;
  }
  efi_simple_file_system_t* file_system = interface;
  efi_file_protocol_t* root = NULL;
  status = file_system->open_volume(file_system, &root);
  if (status != EFI_SUCCESS) {
    console->output_string(console, u"Cannot open boot volume.\r\n");
    return status;
  }
  efi_file_protocol_t* kernel = NULL;
  status = root->open(root, &kernel, u"\\kernel.elf", EFI_FILE_MODE_READ, 0);
  if (status != EFI_SUCCESS) {
    root->close(root);
    console->output_string(console, u"Cannot open kernel.elf.\r\n");
    return status;
  }
  uint8_t header[sizeof(elf64_header_t)];
  uintptr_t size = sizeof(header);
  status = kernel->read(kernel, &size, header);
  kernel->close(kernel);
  root->close(root);
  if (status != EFI_SUCCESS) {
    console->output_string(console, u"Cannot read kernel.elf.\r\n");
    return status;
  }
  if (elf64_validate_header(header, size)) {
    console->output_string(console, u"Kernel ELF header is valid.\r\n");
  } else {
    console->output_string(console, u"Kernel ELF header is invalid.\r\n");
  }
  return EFI_SUCCESS;
}
