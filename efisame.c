
#include <efivar.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

  efi_guid_t guid = EFI_GLOBAL_GUID;

  const char* name = "BootCurrent";
  uint8_t *data;
  size_t data_size;
  uint32_t attr;

  int res = efi_get_variable(guid, name, &data, &data_size, &attr);
  if (res < 0) {
    perror("efi_get_variable");
    exit(1);
  }

  printf("data_size: %zu\n", data_size);

  return 0;
}
