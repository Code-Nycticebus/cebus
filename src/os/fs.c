#include "fs.h"

#include "io.h"
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

static usize file_size(FILE *handle, Error *error) {
  fseek(handle, 0, SEEK_END);
  const long size = ftell(handle);
  fseek(handle, 0, SEEK_SET);
  if (size < 0) {
    Err(error, errno, "Could not get file size: %s", strerror(errno));
    return 0;
  }
  return (usize)size;
}

Bytes file_read_bytes(Str filename, Arena *arena, Error *error) {
  FILE *handle = file_open(filename, "r", error);
  if (error && error->failure) {
    return (Bytes){0};
  }
  usize size = file_size(handle, error);
  if (error && error->failure) {
    return (Bytes){0};
  }
  u8 *buffer = arena_alloc(arena, size);
  io_read(handle, size, buffer, error);
  fclose(handle);
  return bytes_from_parts(size, buffer);
}

Str file_read_content(Str filename, Arena *arena, Error *error) {
  return str_from_bytes(file_read_bytes(filename, arena, error));
}

void file_write(Str filename, Bytes bytes, Error *error) {
  FILE *handle = file_open(filename, "w", error);
  io_write(handle, bytes, error);
  fclose(handle);
}
