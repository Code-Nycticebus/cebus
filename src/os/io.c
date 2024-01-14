#include "io.h"

#include "types/bytes.h"
#include "types/str.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define IO_MAX_INPUT 512

void io_write(FILE *file, Bytes bytes, Error *error) {
  errno = 0;
  fwrite(bytes.data, sizeof(bytes.data[0]), bytes.size, file);
  if (ferror(file)) {
    Err(error, errno, "Could not write file: %s", strerror(errno));
  }
}

Bytes io_read(FILE *file, usize size, u8 *buffer, Error *error) {
  errno = 0;
  const usize bytes_read = fread(buffer, sizeof(u8), size, file);
  if (ferror(file)) {
    Err(error, errno, "Could not read file: %s", strerror(errno));
    return (Bytes){0};
  }
  return bytes_from_parts(bytes_read, buffer);
}

Str io_read_line(FILE *file, usize size, char *buffer, Error *error) {
  fgets(buffer, (int)size, file);
  if (ferror(file)) {
    Err(error, errno, "Could not read line: %s", strerror(errno));
    return (Str){0};
  }
  return str_from_cstr(buffer);
}

Str input(Str prefix) {
  static char buffer[IO_MAX_INPUT] = {0};
  printf(STR_FMT, STR_ARG(prefix));
  fflush(stdout);
  Str res = io_read_line(stdin, IO_MAX_INPUT, buffer, NULL);
  return str_substring(res, 0, res.len - 1);
}