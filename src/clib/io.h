#ifndef __CLIB_IO_H__
#define __CLIB_IO_H__

#include "clib/arena.h"
#include "clib/platform.h"
#include "datatypes/datatypes.h"

#if defined(LINUX)
#include <bits/types/FILE.h>
#elif defined(WINDOWS)
#define _FILE_DEFINED
typedef struct _iobuf FILE;
#endif

typedef enum {
  FILE_OK,
  FILE_OPEN,
} FileError;

typedef struct {
  FILE *handle;
} File;

bool file_try_open(const char *filename, const char *mode, File *file);
File file_open(const char *filename, const char *mode);
void file_close(File *file);

void file_rewind(File *file);

bool file_try_read_bytes(File *file, Arena *arena, Bytes *bytes);
Bytes file_read_bytes(File *file, Arena *arena);

bool file_stream_bytes(File *file, usize chunk_size,
                       void (*stream)(Bytes bytes));

void file_write(File *file, Bytes bytes);

#endif // !__CLIB_IO_H__
