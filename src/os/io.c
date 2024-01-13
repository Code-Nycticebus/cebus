#include "io.h"
#include "types/str.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define IO_MAX_INPUT 512

CLIB_FMT(1, 2)
void print(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  vprintf(fmt, va);
  va_end(va);
}

Str input(Str prefix) {
  static char buffer[IO_MAX_INPUT] = {0};
  print(STR_FMT, STR_ARG(prefix));
  fflush(stdout);
  fgets(buffer, IO_MAX_INPUT, stdin);
  return str_from_parts(strlen(buffer) - 1, buffer);
}
