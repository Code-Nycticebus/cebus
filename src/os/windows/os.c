#include "../os.h"

#if defined(WINDOWS)

#include "types/integers.h"
#include "types/str.h"
#include <direct.h>
#include <string.h>
#include <windows.h>

void os_chdir(Str path) {
  char buffer[MAX_PATH] = {0};
  memcpy(buffer, path.data, usize_min(path.len, MAX_PATH));
  _chdir(buffer);
}

Str os_getcwd(Arena *arena) {
  usize size = GetCurrentDirectory(0, NULL);
  char *buf = arena_alloc(arena, size);
  GetCurrentDirectory((DWORD)size, buf);
  // TODO error handling
  return str_from_parts(size, buf);
}

#endif
