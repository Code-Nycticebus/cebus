#include "string_builder.h"

#include "clib/type/string.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <vadefs.h>

StringBuilder sb_init(Arena *arena) {
  StringBuilder sb = {0};
  da_init(&sb, arena);
  da_push(&sb, 0);
  return sb;
}

Str sb_to_str(StringBuilder *sb) {
  return str_from_parts(sb->len - 1, sb->items);
}

void sb_append_parts(StringBuilder *sb, usize size, const char *s) {
  (void)da_pop(sb);
  da_extend(sb, size, s);
  da_push(sb, 0);
}

void sb_append_cstr(StringBuilder *sb, const char *cstr) {
  sb_append_parts(sb, strlen(cstr), cstr);
}

void sb_append_str(StringBuilder *sb, Str str) {
  sb_append_parts(sb, str.len, str.data);
}

void sb_append_c(StringBuilder *sb, char c) { sb_append_parts(sb, 1, &c); }

usize sb_append_fmt(StringBuilder *sb, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  usize size = (usize)vsnprintf(NULL, 0, fmt, va) + 1;
  va_end(va);

  da_reserve(sb, size);
  va_start(va, fmt);
  vsnprintf(&da_last(sb), size, fmt, va);
  sb->len += size - 1;
  va_end(va);
  return size - 1;
}

usize sb_append_va(StringBuilder *sb, const char *fmt, va_list va) {
  usize size = (usize)vsnprintf(NULL, 0, fmt, va) + 1;

  da_reserve(sb, size);
  vsnprintf(&da_last(sb), size, fmt, va);
  sb->len += size - 1;
  return size - 1;
}
