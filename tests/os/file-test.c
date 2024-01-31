#include "os/fs.h"

#include "collection/da.h"
#include "core/assert.h"
#include "core/defines.h"

int main(void) {
  Arena arena = {0};

  clib_assert(file_exists(STR(__FILE__)), "This file should exist");

  Str filename1 = STR("__test_1_");
  Str filename2 = STR("__test_2_");

  Error *PANIC = ErrPanic;

  file_write(filename1, BYTES(0x69, 0x69), PANIC);
  clib_assert(file_exists(filename1), "This file should exist");

  file_rename(filename1, filename2, PANIC);
  clib_assert(file_exists(filename2), "This file should exist");

  Bytes content = file_read_bytes(filename2, &arena, PANIC);

  clib_assert(content.data[0] == 0x69, "Did not read the file correctly");
  clib_assert(content.data[1] == 0x69, "Did not read the file correctly");

  file_remove(filename2, PANIC);

  clib_assert(file_exists(filename2) == false, "This file should not exist");
  clib_assert(file_exists(filename1) == false, "This file should not exist");

  arena_free(&arena);
}
