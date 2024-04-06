#include "clib/clib.h"
#include "clib/core/assert.h"

int main(void) {
  Arena arena = {0};
  StringBuilder sb = sb_init(&arena);

  sb_append_cstr(&sb, "Hello");
  sb_append_str(&sb, STR(", World"));
  usize size = sb_append_fmt(&sb, " %d", 420);
  clib_assert(size == 4, "Did not return the correct amount!");
  Str s = sb_to_str(&sb);

  clib_assert(str_eq(s, STR("Hello, World 420")), "");

  arena_free(&arena);
}
