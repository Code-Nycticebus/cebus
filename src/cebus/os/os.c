#include "os.h"

#include "cebus/core/debug.h"
#include "cebus/type/integer.h"
#include "cebus/type/string.h"

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

#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void os_chdir(Path path) {
  char pathname[FILENAME_MAX] = {0};
  memcpy(pathname, path.data, usize_min(path.len, FILENAME_MAX));
  cebus_assert(chdir(pathname) == -1, "Could not change directory to '" STR_FMT "': %s",
               STR_ARG(path), strerror(errno));
}

void os_mkdir(Path path) {
  os_mkdir_mode(path, 0755); // NOLINT
}

void os_mkdir_mode(Path path, u32 mode) {
  char pathname[FILENAME_MAX] = {0};
  memcpy(pathname, path.data, usize_min(path.len, FILENAME_MAX));
  mkdir(pathname, mode);
}

Path os_getcwd(Arena *arena) {
  char *buf = arena_alloc(arena, FILENAME_MAX);
  char *ret = getcwd(buf, FILENAME_MAX);
  cebus_assert(ret, "Could not get cwd");
  return str_from_cstr(ret);
}

//////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include <direct.h>
#include <windows.h>

void os_chdir(Path path) {
  char buffer[FILENAME_MAX] = {0};
  memcpy(buffer, path.data, usize_min(path.len, FILENAME_MAX));
  _chdir(buffer);
}

void os_mkdir(Path path) {
  char pathname[FILENAME_MAX] = {0};
  memcpy(pathname, path.data, usize_min(path.len, FILENAME_MAX));
  _mkdir(pathname);
}

void os_mkdir_mode(Path path, u32 mode) {
  (void)mode;
  os_mkdir(path);
}

Path os_getcwd(Arena *arena) {
  DWORD size = GetCurrentDirectory(0, NULL);
  char *buf = arena_alloc(arena, size);
  GetCurrentDirectory((DWORD)size, buf);
  // TODO error handling
  return str_from_parts(size, buf);
}

//////////////////////////////////////////////////////////////////////////////

#endif
