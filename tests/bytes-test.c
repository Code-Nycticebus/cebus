#include "clib/bytes.h"
#include <stdio.h>

int main(void) {
  Arena arena = {0};
  Bytes b = BYTES("\0\0\0\01");

  Bytes bb = bytes_from_hex(STR("FFAAFF"), &arena);

  printf("0x%x\n", *(uint32_t *)b.data);
  printf("0x%x\n", *(uint32_t *)bb.data);

  arena_free(&arena);
}
