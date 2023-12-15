#ifndef __CTOOLS_STR_H__
#define __CTOOLS_STR_H__

#include "arena.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define STR_FMT "%.*s"
#define STR_ARG(str) (int)str.len, str.data

#define STR(str) ((Str){.len = sizeof(str) - 1, .data = str})

typedef struct {
  size_t len;
  const char *data;
} Str;

Str str_from_parts(size_t size, const char *cstr);
Str str_from_cstr(const char *cstr);

Str str_trim_left(Str str);
Str str_trim_right(Str str);
Str str_trim(Str str);

Str str_copy(Arena *arena, Str src);
Str str_cat(Arena *arena, Str s1, Str s2);
bool str_eq(Str s1, Str s2);
bool str_startswith(Str s1, Str prefix);
bool str_endswith(Str s1, Str suffix);
bool str_in_str(Str needle, Str haystack);

bool str_try_chop_by_delim(Str *str, char delim, Str *chunk);
Str str_chop_by_delim(Str *str, char delim);
bool str_try_chop_by_predicate(Str *str, bool (*predicate)(char), Str *chunk);
Str str_chop_by_predicate(Str *str, bool (*predicate)(char));

Str str_u64_to_str(Arena *arena, uint64_t n);
uint64_t str_to_u64(Str str);
uint64_t str_chop_u64(Str *str);

#endif // !__CTOOLS_STR_H__
