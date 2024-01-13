#include "io.h"
#include "types/str.h"

#include <stdio.h>
#include <string.h>

#define IO_MAX_INPUT 512

Str input(Str prefix) {
  static char buffer[IO_MAX_INPUT] = {0};
  printf(STR_FMT, STR_ARG(prefix));
  fflush(stdout);
  fgets(buffer, IO_MAX_INPUT, stdin);
  return str_from_parts(strlen(buffer) - 1, buffer);
}
