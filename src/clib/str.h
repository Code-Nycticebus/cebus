#ifndef __CLIB_STR_H__
#define __CLIB_STR_H__

#include "arena.h"

#include "defines.h"

#define STR_FMT "%.*s"
#define STR_ARG(str) (int)(str).len, (str).data

#define STR(str) ((Str){.len = sizeof(str) - 1, .data = (str)})

#define STR_NOT_FOUND SIZE_MAX

typedef struct {
  size_t len;
  const char *data;
} Str;

Str str_from_parts(size_t size, const char *cstr);
Str str_from_cstr(const char *cstr);

Str str_trim_left(Str str);
Str str_trim_right(Str str);
Str str_trim(Str str);

Str str_copy(Str src, Arena *arena);
Str str_concat(Str s1, Str s2, Arena *arena);
Str str_concat_many(size_t count, Str s[count], Arena *arena);
Str str_upper(Str s, Arena *arena);
Str str_lower(Str s, Arena *arena);
Str str_map(Str s, char (*map_fn)(size_t, char), Arena *arena);
Str str_replace(Str s, Str old, Str new, Arena *arena);

Str str_substring(Str s, size_t idx1, size_t idx2);

bool str_eq(Str s1, Str s2);
bool str_eq_ignorecase(Str s1, Str s2);
bool str_startswith(Str s1, Str prefix);
bool str_endswith(Str s1, Str suffix);
bool str_contains(Str haystack, Str needle);

bool str_try_chop_by_delim(Str *str, char delim, Str *chunk);
Str str_chop_by_delim(Str *str, char delim);
bool str_try_chop_by_predicate(Str *str, bool (*predicate)(char), Str *chunk);
Str str_chop_by_predicate(Str *str, bool (*predicate)(char));

Str str_chop_right_by_delim(Str *str, char delim);
Str str_chop_right_by_predicate(Str *str, bool (*predicate)(char));

Str str_u64(Arena *arena, uint64_t n);
uint64_t str_to_u64(Str str);
uint64_t str_chop_u64(Str *str);

size_t str_find(Str haystack, Str needle);
size_t str_count(Str haystack, Str needle);

#endif // !__CLIB_STR_H__
