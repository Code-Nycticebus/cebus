#include "fs.h"

#include "core/error.h"
#include "io.h"
#include "types/str.h"
#include "types/utf8.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

static FILE *file_open(Str filename, const char *mode, Error *error) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);
  errno = 0;
  FILE *handle = fopen(_filename, mode);
  if (handle == NULL) {
    error_emit(error, errno, "Could not open file: %s: %s", _filename,
               strerror(errno));
  }
  return handle;
}

static usize file_size(FILE *handle, Error *error) {
  fseek(handle, 0, SEEK_END);
  const long size = ftell(handle);
  fseek(handle, 0, SEEK_SET);
  if (size < 0) {
    error_emit(error, errno, "Could not get file size: %s", strerror(errno));
    return 0;
  }
  return (usize)size;
}

////////////////////////////////////////////////////////////////////////////

Bytes file_read_bytes(Str filename, Arena *arena, Error *error) {
  Bytes result = {0};
  FILE *handle = file_open(filename, "r", error);
  if (error && error->failure) {
    goto defer;
  }
  usize size = file_size(handle, error);
  if (error && error->failure) {
    goto defer;
  }

  u8 *buffer = arena_alloc(arena, size);
  result = io_read(handle, size, buffer, error);

defer:
  if (handle) {
    fclose(handle);
  }
  return result;
}

Str file_read_str(Str filename, Arena *arena, Error *error) {
  return str_from_bytes(file_read_bytes(filename, arena, error));
}

Utf8 file_read_utf8(Str filename, Arena *arena, Error *error) {
  return utf8_decode(file_read_bytes(filename, arena, error), error);
}

void file_write(Str filename, Bytes bytes, Error *error) {
  FILE *handle = file_open(filename, "w", error);
  if (error && error->failure) {
    goto defer;
  }
  io_write(handle, bytes, error);
defer:
  if (handle) {
    fclose(handle);
  }
  return;
}

void file_rename(Str old_name, Str new_name, Error *error) {
  char _old_name[FILENAME_MAX] = {0};
  memcpy(_old_name, old_name.data, old_name.len);

  char _new_name[FILENAME_MAX] = {0};
  memcpy(_new_name, new_name.data, new_name.len);

  errno = 0;
  int ret = rename(_old_name, _new_name);
  if (ret == -1) {
    error_emit(error, errno, "Could not rename the file: " STR_FMT ": %s",
               STR_ARG(old_name), strerror(errno));
  }
}

void file_remove(Str filename, Error *error) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);

  errno = 0;
  int ret = remove(_filename);
  if (ret == -1) {
    error_emit(error, errno, "Could not rename the file: " STR_FMT ": %s",
               STR_ARG(filename), strerror(errno));
  }
}

////////////////////////////////////////////////////////////////////////////
