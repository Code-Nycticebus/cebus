#include "../os.h"

#if defined(LINUX)

#include "core/asserts.h"
#include "types/integers.h"
#include "types/str.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

#endif
