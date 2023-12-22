#include <locale.h>
#include <stdio.h>

#include "datatypes/bytes.h"
#include "datatypes/utf8.h"

int main(void) {
  Arena arena = {0};
  setlocale(LC_ALL, "");

  Utf8 s = UTF8("Hellö Wörld 💩");
  printf(UTF8_FMT "\n", UTF8_ARG(s));

  Utf8 s2 = utf8_copy(s, &arena);
  printf(UTF8_FMT "\n", UTF8_ARG(s2));

  Bytes bytes =
      BYTES(0xF0, 0x9F, 0x8E, 0x89, 0xE2, 0x9C, 0x85, 0xF0, 0x9F, 0x98, 0x81);
  Utf8 s3 = utf8_decode(bytes);
  printf(UTF8_FMT "\n", UTF8_ARG(s3));

  Utf8 test = utf8_copy(s3, &arena);

  Utf8 one = utf8_next(&test);
  printf("Rest: " UTF8_FMT "\n", UTF8_ARG(test));
  printf("One: " UTF8_FMT "\n", UTF8_ARG(one));

  one = utf8_next(&test);
  printf("Rest: " UTF8_FMT "\n", UTF8_ARG(test));
  printf("One: " UTF8_FMT "\n", UTF8_ARG(one));

  one = utf8_next(&test);
  printf("Rest: " UTF8_FMT "\n", UTF8_ARG(test));
  printf("One: " UTF8_FMT "\n", UTF8_ARG(one));
  arena_free(&arena);

  one = utf8_next(&test);
  printf("Rest: " UTF8_FMT "\n", UTF8_ARG(test));
  printf("One: " UTF8_FMT "\n", UTF8_ARG(one));
  arena_free(&arena);
}
