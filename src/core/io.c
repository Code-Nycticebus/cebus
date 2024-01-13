#include "io.h"
#include "core/asserts.h"
#include "types/bytes.h"
#include "types/str.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static FILE *file_open(Str filename, const char *mode, Error *error) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);
  errno = 0;
  FILE *handle = fopen(_filename, mode);
  if (handle == NULL) {
    Err(error, errno, "Could not open file: %s: %s", _filename,
        strerror(errno));
  }
  return handle;
}

static usize file_size(FILE *handle) {
  const long current = ftell(handle);
  fseek(handle, 0, SEEK_END);
  const long size = ftell(handle);
  fseek(handle, current, SEEK_SET);
  clib_assert(0 < size, "Could not get file size: %s", strerror(errno));
  return (usize)size;
}

Bytes file_read_bytes(Str filename, Arena *arena, Error *error) {
  FILE *handle = file_open(filename, "r", error);
  usize size = file_size(handle);
  u8 *buffer = arena_alloc(arena, size);
  clearerr(handle);
  const usize bytes_read = fread(buffer, sizeof(u8), size, handle);
  if (ferror(handle)) {
    Err(error, FILE_READ, "Could not read file");
    return (Bytes){0};
  }
  return bytes_from_parts(bytes_read, buffer);
}

void file_write(Str filename, Bytes bytes, Error *error) {
  FILE *handle = file_open(filename, "w", error);
  fwrite(bytes.data, sizeof(u8), bytes.size, handle);
  if (ferror(handle)) {
    Err(error, FILE_WRITE, "Could not write file");
  }
}

Str input(Str prefix, usize max, char *buffer) {
  printf(STR_FMT, STR_ARG(prefix));
  fflush(stdout);
  fgets(buffer, (int)max, stdin);
  return str_from_cstr(buffer);
}
