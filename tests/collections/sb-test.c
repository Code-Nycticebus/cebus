#include "cebus.h"

#include <stdarg.h>

static void sb_va_test(const char *fmt, ...) {
  Arena arena = {0};
  StringBuilder sb = sb_init(&arena);
  va_list va;
  va_start(va, fmt);
  sb_append_va(&sb, fmt, va);
  va_end(va);

  Str s = sb_to_str(&sb);

  cebus_assert(str_eq(s, STR("420 69")), STR_FMT, STR_ARG(s));

  arena_free(&arena);
}

int main(void) {
  Arena arena = {0};
  StringBuilder sb = sb_init(&arena);

  sb_append_cstr(&sb, "Hello");
  sb_append_str(&sb, STR(", World"));
  usize size = sb_append_fmt(&sb, " %d", 420);
  cebus_assert(size == 4, "Did not return the correct amount!");
  sb_append_c(&sb, '!');
  Str s = sb_to_str(&sb);

  cebus_assert(str_eq(s, STR("Hello, World 420!")), STR_FMT, STR_ARG(s));

  sb_va_test("%d %d", 420, 69);

  sb_clear(&sb);
  cebus_assert(sb.len == 0, "Did not reset correctly");

  arena_free(&arena);
}
