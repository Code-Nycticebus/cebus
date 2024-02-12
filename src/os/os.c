#include "os.h"

#include "core/assert.h"
#include "type/integer.h"
#include "type/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

Str os_getenv(const char *env, Error *error) {
  const char *var = getenv(env);
  if (var == NULL) {
    error_emit(error, 0, "Could not get environment variable: %s", env);
    return (Str){0};
  }
  return str_from_cstr(var);
}

////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)

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

//////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include <windows.h>
#include <direct.h>

void os_chdir(Str path) {
  char buffer[FILENAME_MAX] = {0};
  memcpy(buffer, path.data, usize_min(path.len, FILENAME_MAX));
  _chdir(buffer);
}

Str os_getcwd(Arena *arena) {
  DWORD size = GetCurrentDirectory(0, NULL);
  char *buf = arena_alloc(arena, size);
  GetCurrentDirectory((DWORD)size, buf);
  // TODO error handling
  return str_from_parts(size, buf);
}

//////////////////////////////////////////////////////////////////////////////

#endif
