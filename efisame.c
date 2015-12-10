//===-- efisame.c ---------------------------------------------------------===//
//
// Set 'BootNext' efi variable to 'BootCurrent',
// so subsequent reboots continue to use entry most recently used.
//
// Uses efivar and requires root.
//
//===----------------------------------------------------------------------===//
//
// Will Dietz, 2015
//
//===----------------------------------------------------------------------===//

#include <efivar.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  efi_guid_t guid;
  const char *name;
  uint8_t *data;
  size_t data_size;
  uint32_t attr;
} efivar_t;

int get_var(const char *name, efivar_t *out) {
  efivar_t var = {EFI_GLOBAL_GUID, name, NULL, 0, 0};

  int res = efi_get_variable(var.guid, var.name, &var.data, &var.data_size,
                             &var.attr);
  if (res >= 0) {
    *out = var;
  }

  return res;
}

// Return true if variables contain same data
int same_var_data(efivar_t *a, efivar_t *b) {
  return a->data_size == b->data_size &&
         memcmp(a->data, b->data, a->data_size) == 0;
}

// (Attrs used by efibootmgr to set BootNext)
const uint32_t BNEXT_ATTR = EFI_VARIABLE_NON_VOLATILE |
                            EFI_VARIABLE_BOOTSERVICE_ACCESS |
                            EFI_VARIABLE_RUNTIME_ACCESS;

int main() {

  efivar_t bcur, bnext;

  int r = get_var("BootCurrent", &bcur);
  if (r < 0) {
    perror("efi_get_variable(BootCurrent)");
    return EXIT_FAILURE;
  }
  assert(bcur.data_size == 2);

  // Check if "BootNext" is set.
  r = get_var("BootNext", &bnext);
  if (r >= 0) {
    // Already set, ensure it's what we expect:
    if (!same_var_data(&bcur, &bnext)) {
      fprintf(stderr, "Warning: BootNext set to value other than BootCurrent! "
                      "Leaving it as-is...\n");
      return EXIT_FAILURE;
    }
    printf("BootNext already set to BootCurrent.\n");
  } else {
    // Create BootNext variable, set to BootCurrent

    r = efi_set_variable(EFI_GLOBAL_GUID, "BootNext", bcur.data, bcur.data_size,
                         BNEXT_ATTR);
    if (r < 0) {
      perror("Failed to set BootNext, efi_set_variable");
      return EXIT_FAILURE;
    }
    printf("Created BootNext variable set to BootCurrent.\n");
  }

  // Double-check BootNext is what we expect before declaring success.
  r = get_var("BootNext", &bnext);
  if (r < 0) {
    perror("Failed to get BootNext?");
    return EXIT_FAILURE;
  }

  // Check has same data as BootCurrent
  if (!same_var_data(&bcur, &bnext)) {
    fprintf(stderr, "BootNext check failed: data mismatch.");
    return EXIT_FAILURE;
  }

  // And that the attributes are what we expect
  if (bnext.attr != BNEXT_ATTR) {
    fprintf(stderr, "BootNext check failed: expected attributes [%x], got [%x].", BNEXT_ATTR, bnext.attr);
    return EXIT_FAILURE;
  }

  printf("BootNext variable value verified.\n");

  return EXIT_SUCCESS;
}
