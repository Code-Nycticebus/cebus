#include "clib/io.h"
#include "datatypes/bytes.h"
#include "datatypes/str.h"

int main(void) {
  Arena arena = {0};

  File file = file_open(__FILE__, "r");
  Str content = str_from_bytes(file_read_bytes(&file, &arena));
  file_close(&file);

  File same_file = file_open(__FILE__, "w");
  for (Str line = {0}, s = content; str_try_chop_by_delim(&s, '\n', &line);) {
    file_write(&same_file, str_to_bytes(line));
    file_write(&same_file, BYTES_STR("\n"));
  }
  file_close(&same_file);

  arena_free(&arena);
}
