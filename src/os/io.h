#ifndef __CLIB_IO_H__
#define __CLIB_IO_H__

#include "core/defines.h"
#include "core/error.h"

#if defined(LINUX)
#include <bits/types/FILE.h>
#elif defined(WINDOWS)
#define _FILE_DEFINED
typedef struct _iobuf FILE;
#endif

void io_write(FILE *file, Bytes bytes, Error *error);
Bytes io_read(FILE *file, usize size, u8 *buffer, Error *error);
Str io_read_line(FILE *file, usize size, char *buffer, Error *error);

Str input(Str prefix);

#endif // !__CLIB_IO_H__
