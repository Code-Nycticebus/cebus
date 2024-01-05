#include "io.h"
#include "clib/asserts.h"
#include "datatypes/bytes.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

bool file_try_open(const char *filename, const char *mode, File *file) {
  file->handle = fopen(filename, mode);
  clib_assert_return(file->handle, false);
  return true;
}

File file_open(const char *filename, const char *mode) {
  File file = {0};
  clib_assert(file_try_open(filename, mode, &file),
              "Could not open file: %s: %s", filename, strerror(errno));
  return file;
}

void file_close(File *file) {
  fclose(file->handle);
  file->handle = NULL;
}

void file_rewind(File *file) {
  clib_assert_debug(file->handle, "Invalid file handle!");
  rewind(file->handle);
}

usize file_size(File *file) {
  const long current = ftell(file->handle);
  fseek(file->handle, 0, SEEK_END);
  const long size = ftell(file->handle);
  fseek(file->handle, current, SEEK_SET);
  clib_assert(0 < size, "Could not get file size: %s", strerror(errno));
  return (usize)size;
}

bool file_try_read_bytes(File *file, Arena *arena, Bytes *bytes) {
  clib_assert_debug(file->handle, "Invalid file handle!");
  const usize size = file_size(file);
  u8 *buffer = arena_alloc(arena, size);
  const usize bytes_read = fread(buffer, sizeof(u8), size, file->handle);
  if (ferror(file->handle)) {
    return false;
  }
  *bytes = bytes_from_parts(bytes_read, buffer);
  return true;
}

Bytes file_read_bytes(File *file, Arena *arena) {
  Bytes bytes = {0};
  clib_assert(file_try_read_bytes(file, arena, &bytes),
              "Could not read file: %s\n", strerror(errno));
  return bytes;
}

bool file_stream_bytes(File *file, usize chunk_size,
                       void (*stream)(Bytes bytes)) {
  clib_assert_debug(file->handle, "Invalid file handle!");
  Arena arena = {0};

  u8 *buffer = arena_alloc(&arena, chunk_size);
  while (!feof(file->handle)) {
    const usize size = fread(buffer, sizeof(u8), chunk_size, file->handle);
    if (ferror(file->handle)) {
      return false;
    }
    stream(bytes_from_parts(size, buffer));
  }

  arena_free(&arena);
  return true;
}

void file_write(File *file, Bytes bytes) {
  clib_assert_debug(file->handle, "Invalid file handle!");
  fwrite(bytes.data, sizeof(u8), bytes.size, file->handle);
  clib_assert(!ferror(file->handle), "Could not write file: %s",
              strerror(errno));
}
