#include <uefi/uefi.h>

efi_status_t EFIAPI
efi_main(efi_handle_t image_handle, efi_system_table_t* system_table) {
  (void)image_handle;
  (void)system_table;

  return EFI_SUCCESS;
}
