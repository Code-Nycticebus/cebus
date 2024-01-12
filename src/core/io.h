#ifndef __CLIB_IO_H__
#define __CLIB_IO_H__

#include "core/arena.h"
#include "core/defines.h"
#include "core/error.h"
#include "core/platform.h"

#if defined(LINUX)
#include <bits/types/FILE.h>
#elif defined(WINDOWS)
#define _FILE_DEFINED
typedef struct _iobuf FILE;
#endif

#include <errno.h>

typedef enum {
  FILE_OK,
  FILE_PERMISSION = EACCES,
  FILE_NOT_FOUND = ENOENT,
  FILE_READ,
  FILE_WRITE,
} FileError;

typedef struct {
  FILE *handle;
} File;

File file_open(const char *filename, const char *mode, Error *error);
void file_close(File *file);

usize file_size(File *file);
void file_rewind(File *file);

Bytes file_read_bytes(File *file, Arena *arena, Error *error);

bool file_stream_bytes(File *file, usize chunk_size,
                       void (*stream)(Bytes bytes));

void file_write(File *file, Bytes bytes, Error *error);

#endif // !__CLIB_IO_H__
