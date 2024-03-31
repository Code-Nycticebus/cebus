/* DOCUMENTATION
## Functions

- **Reading Files**:
  - `file_read_bytes(filename, arena, error)`: Reads the entire file into a byte
array.
  - `file_read_str(filename, arena, error)`: Reads the entire file into a
string.
  - `file_read_utf8(filename, arena, error)`: Reads the entire file into UTF-8
format.

- **Writing Files**:
  - `file_write_bytes(filename, bytes, error)`: Writes byte data to a file.
  - `file_write_str(filename, content, error)`: Writes a string to a file.
  - `file_write_utf8(filename, content, error)`: Writes UTF-8 formatted data to
a file.

- **File Management**:
  - `file_open(filename, mode, error)`: Opens a file with the specified mode.
  - `file_close(file, error)`: Closes an open file.
  - `file_rename(old_name, new_name, error)`: Renames a file.
  - `file_remove(filename, error)`: Removes a file.
  - `file_exists(filename)`: Checks if a file exists.

## Usage Example

```c
Arena arena = {0};
Error error = ErrNew;
Str content = file_read_str(STR("filename.txt"), &arena, &error);
error_context(&error, {
    error_raise();
});
arena_free(&arena);
```
*/

#ifndef __CLIB_FS_H__
#define __CLIB_FS_H__

#include "clib/core/arena.h"
#include "clib/core/defines.h"
#include "clib/core/error.h"

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

void file_write_bytes(Str filename, Bytes bytes, Error *error);
void file_write_str(Str filename, Str content, Error *error);
void file_write_utf8(Str filename, Utf8 content, Error *error);

void file_rename(Str old_name, Str new_name, Error *error);
void file_remove(Str filename, Error *error);
bool file_exists(Str filename);

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_FS_H__ */
