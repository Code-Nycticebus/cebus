#include "cebus/os/fs.h"

#include "cebus/collection/da.h"
#include "cebus/core/debug.h"
#include "cebus/core/defines.h"

int main(void) {
  Arena arena = {0};
  Str dir = STR(".");
  FsIterator it = fs_iter_begin(dir, true, &arena, ErrPanic);
  for (u32 i = 0; fs_iter_end(&it); fs_iter_next(&it), ++i) {
    cebus_log_debug("%d: " STR_FMT, i, STR_ARG(it.current.path));
  }

  arena_free(&arena);
}
