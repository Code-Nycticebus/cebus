#include <locale.h>
#include <stdio.h>

#include "clib/utf-8.h"

int main(void) {
  setlocale(LC_ALL, "");
  Arena arena = {0};

  Utf8 s = UTF8("Hellö Wörld 💩");
  printf(UTF8_FMT "\n", UTF8_ARG(s));

  Utf8 s2 = utf8_from_cstr("What the hell", &arena);
  printf(UTF8_FMT "\n", UTF8_ARG(s2));

  Bytes bytes = BYTES(0xF0, 0x9F, 0x8E, 0x89);
  Utf8 s3 = utf8_decode(bytes, &arena);
  printf(UTF8_FMT "\n", UTF8_ARG(s3));

  arena_free(&arena);
}
