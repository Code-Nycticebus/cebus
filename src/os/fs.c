#include "fs.h"

#include "core/error.h"
#include "io.h"
#include "types/string.h"
#include "types/utf8.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

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

FILE *file_open(Str filename, const char *mode, Error *error) {
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

void file_close(FILE *file, Error *error) {
  if (file == NULL) {
    error_emit(error, FILE_INVALID, "can't close a FILE* that is NULL");
    return;
  }
  errno = 0;
  int ret = fclose(file);
  if (ret == EOF) {
    error_emit(error, errno, "closing file failed: %s", strerror(errno));
  }
}

Bytes file_read_bytes(Str filename, Arena *arena, Error *error) {
  Bytes result = {0};
  FILE *handle = file_open(filename, "r", error);
  error_propagate(error, { goto defer; });
  usize size = file_size(handle, error);
  error_propagate(error, { goto defer; });

  u8 *buffer = arena_alloc(arena, size);
  result = io_read(handle, size, buffer, error);
  error_propagate(error, { goto defer; });

defer:
  if (handle) {
    fclose(handle);
  }
  return result;
}

Str file_read_str(Str filename, Arena *arena, Error *error) {
  Bytes bytes = file_read_bytes(filename, arena, error);
  error_propagate(error, { return (Str){0}; });
  return str_from_bytes(bytes);
}

Utf8 file_read_utf8(Str filename, Arena *arena, Error *error) {
  Utf8 res = {0};
  Bytes bytes = file_read_bytes(filename, arena, error);
  error_propagate(error, { return (Utf8){0}; });
  res = utf8_decode(bytes, error);
  error_propagate(error, { return (Utf8){0}; });
  return res;
}

void file_write(Str filename, Bytes bytes, Error *error) {
  FILE *handle = file_open(filename, "w", error);
  error_propagate(error, { goto defer; });

  io_write(handle, bytes, error);
  error_propagate(error, { goto defer; });

defer:
  if (handle) {
    fclose(handle);
  }
}

void file_write_str(Str filename, Str content, Error *error) {
  file_write(filename, str_to_bytes(content), error);
}

void file_write_utf8(Str filename, Utf8 content, Error *error) {
  Bytes bytes = utf8_encode(content, error);
  error_propagate(error, { return; });
  file_write(filename, bytes, error);
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
