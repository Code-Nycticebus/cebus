#include "core/platform.h" // IWYU pragma: keep always
#include "fs.h"            // IWYU pragma: keep always
#include "os.h"            // IWYU pragma: keep always

#if defined(LINUX)

#include "core/asserts.h"
#include "type/integer.h"
#include "type/string.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////////////

void os_chdir(Str path) {
  char pathname[FILENAME_MAX] = {0};
  memcpy(pathname, path.data, usize_min(path.len, FILENAME_MAX));
  chdir(pathname);
}

Str os_getcwd(Arena *arena) {
  char *buf = arena_alloc(arena, FILENAME_MAX);
  char *ret = getcwd(buf, FILENAME_MAX);
  clib_assert(ret, "Could not get cwd");
  return str_from_cstr(ret);
}

//////////////////////////////////////////////////////////////////////////////

bool file_exists(Str filename) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);
  return access(_filename, 0) == 0;
}

//////////////////////////////////////////////////////////////////////////////

#endif
