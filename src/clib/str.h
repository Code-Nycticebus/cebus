#ifndef __CLIB_STR_H__
#define __CLIB_STR_H__

#include "arena.h"

#include "defines.h"

// TODO add all these
//      "https://docs.python.org/3/library/stdtypes.html#str.isalnum"
// TODO make the arguments name uniform (s or str pick one)
// TODO str_insert(Str s, size_t idx, Str src, Arena* arena);
// TODO str_replace_at(Str s, size_t idx, Str new, Arena* arena);
// TODO Str str_reverse(Str s, Arena *arena);

#define STR_FMT "%.*s"
#define STR_ARG(str) (int)(str).len, (str).data

#define STR(str) ((Str){.len = sizeof(str) - 1, .data = (str)})

#define STR_NOT_FOUND SIZE_MAX

#define STR_LETTERS STR("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define STR_UPPERCASE STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define STR_LOWERCASE STR("abcdefghijklmnopqrstuvwxyz")
#define STR_DIGITS STR("0123456789")
#define STR_HEXDIGITS STR("0123456789abcdefABCDEF")
#define STR_PUNCTUATION STR("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~")
#define STR_WHITESPACE STR(" \t\n\r\x0b\x0c")

typedef struct {
  size_t len;
  const char *data;
} Str;

Str str_from_parts(size_t size, const char *cstr);
Str str_from_cstr(const char *cstr);

char str_getc(Str s, size_t idx);

Str str_trim_left(Str str);
Str str_trim_right(Str str);
Str str_trim(Str str);

Str str_copy(Str src, Arena *arena);
Str str_concat(Str s1, Str s2, Arena *arena);
Str str_join(Str sep, size_t count, Str s[count], Arena *arena);
Str str_upper(Str s, Arena *arena);
Str str_lower(Str s, Arena *arena);
Str str_map(Str s, char (*map_fn)(size_t, char), Arena *arena);
Str str_replace(Str s, Str old, Str new, Arena *arena);
Str str_center(Str str, size_t width, char fillchar, Arena *arena);
Str str_ljust(Str str, size_t width, char fillchar, Arena *arena);
Str str_rjust(Str str, size_t width, char fillchar, Arena *arena);
Str str_repeat(Str s, size_t count, Arena *arena);
Str str_reverse(Str s, Arena *arena);

Str str_substring(Str s, size_t idx1, size_t idx2);

bool str_eq(Str s1, Str s2);
bool str_eq_ignorecase(Str s1, Str s2);
bool str_startswith(Str s1, Str prefix);
bool str_endswith(Str s1, Str suffix);
bool str_contains(Str haystack, Str needle);
CmpOrdering str_compare(Str s1, Str s2);
CmpOrdering str_compare_qsort(const void *s1, const void *s2);

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
size_t str_find_last(Str haystack, Str needle);
size_t str_count(Str haystack, Str needle);

#endif // !__CLIB_STR_H__
