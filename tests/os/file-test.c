#include "cebus/os/fs.h"

#include "cebus/collection/da.h"
#include "cebus/core/debug.h"
#include "cebus/core/defines.h"

int main(void) {
  Arena arena = {0};

  cebus_assert(fs_exists(STR(__FILE__)), "This file should exist");

  Path filename1 = PATH("__test_1_");
  Path filename2 = PATH("__test_2_");

  Error *PANIC = ErrPanic;

  fs_file_write_bytes(filename1, BYTES(0x69, 0x69), PANIC);
  cebus_assert(fs_exists(filename1), "This file should exist");

  fs_rename(filename1, filename2, PANIC);
  cebus_assert(fs_exists(filename2), "This file should exist");

  Bytes content = fs_file_read_bytes(filename2, &arena, PANIC);

  cebus_assert(content.data[0] == 0x69, "Did not read the file correctly");
  cebus_assert(content.data[1] == 0x69, "Did not read the file correctly");

  fs_remove(filename2, PANIC);

  cebus_assert(fs_exists(filename2) == false, "This file should not exist");
  cebus_assert(fs_exists(filename1) == false, "This file should not exist");

  arena_free(&arena);
}
