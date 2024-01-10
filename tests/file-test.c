#include "clib/io.h"
#include "core/defines.h"
#include "types/bytes.h"
#include "types/str.h"

int main(void) {
  Arena arena = {0};

  File file = file_open(__FILE__, "r", NULL);
  Bytes content = file_read_bytes(&file, &arena, NULL);
  file_close(&file);

  File same_file = file_open(__FILE__, "w", NULL);
  for (Str line = {0}, s = str_from_bytes(content);
       str_try_chop_by_delim(&s, '\n', &line);) {
    file_write(&same_file, str_to_bytes(line), NULL);
    file_write(&same_file, BYTES_STR("\n"), NULL);
  }
  file_close(&same_file);

  arena_free(&arena);
}
