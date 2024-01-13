#ifndef __CLIB_FILES_H__
#define __CLIB_FILES_H__

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
  int NOTHING;
} FileInfo;

Bytes file_read_bytes(Str filename, Arena *arena, Error *error);
void file_write(Str filename, Bytes bytes, Error *error);

#endif // !__CLIB_FILES_H__
