/* DOCUMENTATION
### Usage
To read in the entire file as Str
```c
Arena arena = {0};
Error error = ErrCreate;
Str content = file_read_str(STR("filename.txt"), &arena, &error);
if (error_occured(&error)) {
  error_raise(&error);
}
arena_free(&arena);
```
*/

#ifndef __CLIB_FS_H__
#define __CLIB_FS_H__

#include "core/arena.h"
#include "core/defines.h"
#include "core/error.h"

#include <errno.h>

////////////////////////////////////////////////////////////////////////////

typedef enum {
  FILE_OK,
  FILE_PERMISSION = EACCES,
  FILE_NOT_FOUND = ENOENT,
  FILE_INVALID = EBADF,
} FileError;

////////////////////////////////////////////////////////////////////////////

FILE *file_open(Str filename, const char *mode, Error *error);
void file_close(FILE *file, Error *error);

Bytes file_read_bytes(Str filename, Arena *arena, Error *error);
Str file_read_str(Str filename, Arena *arena, Error *error);
Utf8 file_read_utf8(Str filename, Arena *arena, Error *error);

void file_write(Str filename, Bytes bytes, Error *error);
void file_write_str(Str filename, Str content, Error *error);
void file_write_utf8(Str filename, Utf8 content, Error *error);

void file_rename(Str old_name, Str new_name, Error *error);
void file_remove(Str filename, Error *error);
bool file_exists(Str filename);

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_FS_H__ */
