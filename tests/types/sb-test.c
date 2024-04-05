#include "clib/clib.h"
#include "clib/collection/da.h"
#include "clib/core/defines.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  DA(char) buffer;
} StringBuilder;

static StringBuilder sb_init(Arena *arena) {
  StringBuilder sb = {0};
  da_init(&sb.buffer, arena);
  da_push(&sb.buffer, 0);
  return sb;
}

static Str sb_to_str(StringBuilder *sb) {
  return str_from_parts(sb->buffer.len - 1, sb->buffer.items);
}

static void sb_append_parts(StringBuilder *sb, usize size, const char *s) {
  da_pop(&sb->buffer);
  da_extend(&sb->buffer, size, s);
  da_push(&sb->buffer, 0);
}

static void sb_append_cstr(StringBuilder *sb, const char *cstr) {
  sb_append_parts(sb, strlen(cstr), cstr);
}

static void sb_append_str(StringBuilder *sb, Str str) {
  sb_append_parts(sb, str.len, str.data);
}

static void sb_append_fmt(StringBuilder *sb, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  usize size = (usize)vsnprintf(NULL, 0, fmt, va) + 1;
  va_end(va);

  da_reserve(&sb->buffer, size);
  va_start(va, fmt);
  vsnprintf(&da_last(&sb->buffer), size, fmt, va);
  sb->buffer.len += size;
  va_end(va);
}

int main(void) {
  Arena arena = {0};
  StringBuilder sb = sb_init(&arena);

  (void)sb_append_cstr, (void)sb_append_str;
  sb_append_cstr(&sb, "Hello");
  sb_append_str(&sb, STR(", "));
  sb_append_cstr(&sb, "World ");
  sb_append_fmt(&sb, "WHAT: %d", 420);
  Str s = sb_to_str(&sb);
  clib_log_info(STR_FMT, STR_ARG(s));

  arena_free(&arena);
}
