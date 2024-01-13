#ifndef __CLIB_IO_H__
#define __CLIB_IO_H__

#include "core/arena.h"
#include "core/defines.h"
#include "core/error.h"

#include <errno.h>

typedef enum {
  FILE_OK,
  FILE_PERMISSION = EACCES,
  FILE_NOT_FOUND = ENOENT,
  FILE_READ,
  FILE_WRITE,
} FileError;

typedef struct {
  int TODO;
} FileInfo;

Bytes file_read_bytes(Str filename, Arena *arena, Error *error);
// void file_stream_bytes(Str name, void (*stream)(void*, Bytes), void* ctx);
void file_write(Str filename, Bytes bytes, Error *error);

// FileInfo file_info(Str filename);
// bool file_exists(Str filename);

// void file_rename(Str old_name, Str new_name);
// void file_remove(Str filename);

// Directories

Str input(Str prefix);

#endif // !__CLIB_IO_H__
