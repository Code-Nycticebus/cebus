#ifndef __CLIB_STR_H__
#define __CLIB_STR_H__

#include "core/arena.h"

#include "core/defines.h" // IWYU pragma: private: include "str.h"

// TODO add all these
//      "https://docs.python.org/3/library/stdtypes.html#str.isalnum"
// TODO str_insert(Str s, usize idx, Str src, Arena* arena);
// TODO str_replace_at(Str s, usize idx, Str new, Arena* arena);

#define STR_FMT "%.*s"
#define STR_ARG(str) (i32)(str).len, (str).data

#define STR(str) ((Str){.len = sizeof(str) - 1, .data = (str)})

#define STR_NOT_FOUND SIZE_MAX

#define STR_LETTERS STR("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define STR_UPPERCASE STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define STR_LOWERCASE STR("abcdefghijklmnopqrstuvwxyz")
#define STR_DIGITS STR("0123456789")
#define STR_HEXDIGITS STR("0123456789abcdefABCDEF")
#define STR_PUNCTUATION STR("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~")
#define STR_WHITESPACE STR(" \t\n\r\x0b\x0c")

Str str_from_parts(usize size, const char *cstr);
Str str_from_bytes(Bytes bytes);
Bytes str_to_bytes(Str s);
Str str_from_cstr(const char *cstr);

char str_getc(Str s, usize idx);

Str str_trim_left(Str s);
Str str_trim_right(Str s);
Str str_trim(Str s);

Str str_copy(Str s, Arena *arena);
Str str_append(Str s1, Str s2, Arena *arena);
Str str_prepend(Str s1, Str s2, Arena *arena);
Str str_join(Str sep, usize count, Str s[count], Arena *arena);
Str str_upper(Str s, Arena *arena);
Str str_lower(Str s, Arena *arena);
Str str_replace(Str s, Str old, Str new, Arena *arena);
Str str_center(Str s, usize width, char fillchar, Arena *arena);
Str str_ljust(Str s, usize width, char fillchar, Arena *arena);
Str str_rjust(Str s, usize width, char fillchar, Arena *arena);
Str str_repeat(Str s, usize count, Arena *arena);
Str str_reverse(Str s, Arena *arena);

Str str_substring(Str s, usize idx1, usize idx2);

bool str_eq(Str s1, Str s2);
bool str_eq_ignorecase(Str s1, Str s2);
bool str_startswith(Str s1, Str prefix);
bool str_endswith(Str s1, Str suffix);
bool str_contains(Str haystack, Str needle);
bool str_empty(Str s);
CmpOrdering str_compare_gt(Str s1, Str s2);
CmpOrdering str_compare_lt(Str s1, Str s2);
CompareFn str_compare_qsort(CmpOrdering ordering);

bool str_try_chop_by_delim(Str *s, char delim, Str *chunk);
Str str_chop_by_delim(Str *s, char delim);
bool str_try_chop_by_predicate(Str *s, bool (*predicate)(char), Str *chunk);
Str str_chop_by_predicate(Str *s, bool (*predicate)(char));

Str str_chop_right_by_delim(Str *s, char delim);
Str str_chop_right_by_predicate(Str *s, bool (*predicate)(char));

Str str_take(Str *s, usize n);
bool str_try_take(Str *s, usize n, Str *chunk);

Str str_u64(Arena *arena, u64 n);
u64 str_to_u64(Str s);
u64 str_chop_u64(Str *s);

usize str_find(Str haystack, Str needle);
usize str_find_last(Str haystack, Str needle);
usize str_count(Str haystack, Str needle);

u64 str_hash(Str s);

#endif /* !__CLIB_STR_H__ */
