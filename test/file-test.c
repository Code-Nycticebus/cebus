#include "clib/io.h"
#include "clib/logging.h"
#include "datatypes/bytes.h"
#include "datatypes/str.h"

#define LINE_MAX 512

void stream(Bytes bytes) {
  Str s = str_from_bytes(bytes);
  printf("Chunk: \n" STR_FMT "\n", STR_ARG(s));
}

int main(void) {
  Arena arena = {0};

  File file = file_open("test/file-test.c", "r");

  Bytes bytes = file_read_bytes(&file, &arena);

  Str b = bytes_hex(bytes, &arena);
  printf(STR_FMT "\n", STR_ARG(b));

  arena_free(&arena);
}
