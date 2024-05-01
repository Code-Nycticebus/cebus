#include "io.h"

#include "clib/type/byte.h"
#include "clib/type/string.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

#define IO_MAX_INPUT 512

////////////////////////////////////////////////////////////////////////////

usize io_write_fmt(FILE *file, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  i32 size = vfprintf(file, fmt, va);
  va_end(va);
  return (usize)size;
}

void io_write_bytes(FILE *file, Bytes bytes, Error *error) {
  errno = 0;
  fwrite(bytes.data, sizeof(bytes.data[0]), bytes.size, file);
  if (ferror(file)) {
    error_emit(error, errno, "Could not write file: %s", strerror(errno));
  }
}

void io_write_str(FILE *file, Str string, Error *error) {
  io_write_bytes(file, str_to_bytes(string), error);
}

Bytes io_read_bytes(FILE *file, usize size, void *buffer, Error *error) {
  errno = 0;
  const usize bytes_read = fread(buffer, sizeof(u8), size, file);
  if (ferror(file)) {
    error_emit(error, errno, "Could not read file: %s", strerror(errno));
    return (Bytes){0};
  }
  return bytes_from_parts(bytes_read, buffer);
}

Str io_read_line(FILE *file, usize size, char *buffer, Error *error) {
  clearerr(file);
  if (fgets(buffer, (int)size, file) == NULL) {
    if (ferror(file)) {
      error_emit(error, errno, "Could not read line: %s", strerror(errno));
      return (Str){0};
    }
    error_emit(error, EOF, "EOF");
    return (Str){0};
  }
  return str_from_cstr(buffer);
}

////////////////////////////////////////////////////////////////////////////

Str input(Str prefix) {
  static char buffer[IO_MAX_INPUT] = {0};
  printf(STR_FMT, STR_ARG(prefix));
  fflush(stdout);
  Str res = io_read_line(stdin, IO_MAX_INPUT, buffer, NULL);
  return str_substring(res, 0, res.len - 1);
}

////////////////////////////////////////////////////////////////////////////
