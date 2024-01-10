#include "io.h"
#include "clib/asserts.h"
#include "types/bytes.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

File file_open(const char *filename, const char *mode, Error *error) {
  File file = {0};
  errno = 0;
  file.handle = fopen(filename, mode);
  if (file.handle == NULL) {
    Err(error, errno, "Could not open file: %s: %s", filename, strerror(errno));
  }
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

Bytes file_read_bytes(File *file, Arena *arena, Error *error) {
  clib_assert_debug(file->handle, "Invalid file handle!");
  const usize size = file_size(file);
  u8 *buffer = arena_alloc(arena, size);
  clearerr(file->handle);
  const usize bytes_read = fread(buffer, sizeof(u8), size, file->handle);
  if (ferror(file->handle)) {
    Err(error, FILE_READ, "Could not read file\n");
    return (Bytes){0};
  }
  return bytes_from_parts(bytes_read, buffer);
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

void file_write(File *file, Bytes bytes, Error *error) {
  clib_assert_debug(file->handle, "Invalid file handle!");
  fwrite(bytes.data, sizeof(u8), bytes.size, file->handle);
  if (ferror(file->handle)) {
    Err(error, FILE_WRITE, "Could not write file");
  }
}
