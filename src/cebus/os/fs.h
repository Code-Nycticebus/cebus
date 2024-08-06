/* DOCUMENTATION
## Functions

- **Reading Files**:
  - `fs_file_read_bytes(filename, arena, error)`: Reads the entire file into a
byte array.
  - `fs_file_read_str(filename, arena, error)`: Reads the entire file into a
string.
  - `fs_file_read_utf8(filename, arena, error)`: Reads the entire file into
UTF-8 format.

- **Writing Files**:
  - `fs_file_write_bytes(filename, bytes, error)`: Writes byte data to a file.
  - `fs_file_write_str(filename, content, error)`: Writes a string to a file.
  - `fs_file_write_utf8(filename, content, error)`: Writes UTF-8 formatted data
to a file.

- **File Management**:
  - `fs_file_open(filename, mode, error)`: Opens a file with the specified mode.
  - `fs_file_close(file, error)`: Closes an open file.
  - `fs_rename(old_name, new_name, error)`: Renames a file.
  - `fs_remove(filename, error)`: Removes a file.
  - `fs_exists(filename)`: Checks if a file exists.

## Usage Example

```c
Arena arena = {0};
Error error = ErrNew;
Str content = fs_file_read_str(STR("filename.txt"), &arena, &error);
error_context(&error, {
    error_raise();
});
arena_free(&arena);
```


*/

#ifndef __CEBUS_FS_H__
#define __CEBUS_FS_H__

#include "cebus/core/arena.h"
#include "cebus/core/defines.h"
#include "cebus/core/error.h"

#include <errno.h>

////////////////////////////////////////////////////////////////////////////

typedef enum {
  FS_OK,
  FS_PERMISSION = EACCES,
  FS_NOT_FOUND = ENOENT,
  FS_INVALID = EBADF,
} FileError;

////////////////////////////////////////////////////////////////////////////

FILE *fs_file_open(Path path, const char *mode, Error *error);
void fs_file_close(FILE *file, Error *error);

Bytes fs_file_read_bytes(Path filename, Arena *arena, Error *error);
Str fs_file_read_str(Path filename, Arena *arena, Error *error);
Utf8 fs_file_read_utf8(Path filename, Arena *arena, Error *error);

void fs_file_write_bytes(Path filename, Bytes bytes, Error *error);
void fs_file_write_str(Path filename, Str content, Error *error);
void fs_file_write_utf8(Path filename, Utf8 content, Error *error);

void fs_rename(Path old_path, Path new_path, Error *error);
void fs_remove(Path path, Error *error);
bool fs_exists(Path path);
bool fs_is_dir(Path path);

////////////////////////////////////////////////////////////////////////////

/* DOCUMENTATION
## Directory Iteration

```c
int main(void) {
  // initialize and configure iterator
  FsIter it = fs_iter_begin(STR("."), true);

  // iterate over directory with certain filters
  while (fs_iter_next_extension(&it, STR(".clangd"))) {
    // every allocation in the scratch buffer gets reset after each iteration
    Str data = fs_file_read_str(it.current.path, &it.scratch, &it.error);

    // do not return before you call 'fs_iter_end'
    error_propagate(&it.error, { break; });

    // do something with data...
    cebus_log_debug(STR_FMT, STR_ARG(data));
  }

  // collect errors and deinitializes iterator
  Error err = ErrNew;
  fs_iter_end(&it, &err);
  error_context(&err, { error_panic(); });
}
```
 * */

typedef struct {
  bool is_dir;
  Path path;
} FsEntity;

typedef struct {
  Error error;
  Arena scratch;
  bool recursive;
  FsEntity current;
  void *_stack;
} FsIter;

FsIter fs_iter_begin(Path directory, bool recursive);
void fs_iter_end(FsIter *it, Error *error);

bool fs_iter_next(FsIter *it);
bool fs_iter_next_filter(FsIter *it, bool (*filter)(FsEntity *entity));
bool fs_iter_next_directory(FsIter *it);
bool fs_iter_next_files(FsIter *it);
bool fs_iter_next_suffix(FsIter *it, Str suffix);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_FS_H__ */
