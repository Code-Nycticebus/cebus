#include "string_builder.h"

#include "cebus/type/string.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

StringBuilder sb_init(Arena *arena) {
  StringBuilder sb = {0};
  da_init(&sb, arena);
  return sb;
}

void sb_clear(StringBuilder *sb) { da_clear(sb); }

Str sb_to_str(StringBuilder *sb) { return str_from_parts(sb->len, sb->items); }

void sb_append_parts(StringBuilder *sb, usize size, const char *s) { da_extend(sb, size, s); }

void sb_append_cstr(StringBuilder *sb, const char *cstr) { da_extend(sb, strlen(cstr), cstr); }

void sb_append_str(StringBuilder *sb, Str str) { da_extend(sb, str.len, str.data); }

void sb_append_c(StringBuilder *sb, char c) { da_push(sb, c); }

usize sb_append_fmt(StringBuilder *sb, const char *fmt, ...) {
  va_list va;

  // get the size without '\0'
  va_start(va, fmt);
  usize size = (usize)vsnprintf(NULL, 0, fmt, va);
  va_end(va);

  // allocate +1 so it does not overwrite
  da_reserve(sb, size + 1);

  va_start(va, fmt);
  vsnprintf(&da_last(sb) + 1, size + 1, fmt, va);
  va_end(va);
  sb->len += size;

  return size;
}

usize sb_append_va(StringBuilder *sb, const char *fmt, va_list va) {
  // copy the va_list
  va_list va2;
  va_copy(va2, va);

  // get the size without '\0'
  usize size = (usize)vsnprintf(NULL, 0, fmt, va);

  // allocate +1 so it does not overwrite
  da_reserve(sb, size + 1);

  // write into buffer
  vsnprintf(&da_last(sb) + 1, size + 1, fmt, va2);
  sb->len += size;

  return size;
}
