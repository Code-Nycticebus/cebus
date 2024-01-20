#ifndef __CLIB_STR_H__
#define __CLIB_STR_H__

/* DOCUMENTATION
## Usage
Create a new Str with:
```c
Str str = STR("Hello World");
```

You can print the strings using the STR_FMT and STR_ARG() macro:
```c
printf(STR_FMT"\n", STR_ARG(str));
```

I always treat strings as immutable.
So you always have to provide an Arena to do allocations in.
```c
Arena arena = {0};
Str new_str = str_lower(str, &arena);
arena_free(&arena);
```


Iterating over lines is easy
```c
Str content = STR(
    "This is a line\n"
    "This is another line\n"
    "This is the Last Line");
for (Str line = {0}; str_try_chop_by_delim(&content, '\n', &line)) {
  printf("LINE: \""STR_FMT"\"\n", STR_ARG(line));
}
```
Outputs:
```console
LINE: "This is a line"
LINE: "This is another line"
LINE: "This is the Last Line"
```
*/

#include "core/arena.h"
#include "core/defines.h" // IWYU pragma: private: include "str.h"

// CONSTANTS ///////////////////////////////////////

#define STR_NOT_FOUND SIZE_MAX

#define STR_LETTERS STR("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define STR_UPPERCASE STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define STR_LOWERCASE STR("abcdefghijklmnopqrstuvwxyz")
#define STR_DIGITS STR("0123456789")
#define STR_HEXDIGITS STR("0123456789abcdefABCDEF")
#define STR_PUNCTUATION STR("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~")
#define STR_WHITESPACE STR(" \t\n\r\x0b\x0c")

// CONSTANTS END ///////////////////////////////////////

// CONSTRUCTORS ///////////////////////////////////////

Str str_from_parts(usize size, const char *cstr);
Str str_from_bytes(Bytes bytes);
Bytes str_to_bytes(Str s);
Str str_from_cstr(const char *cstr);

// CONSTRUCTORS END ///////////////////////////////////////

// MANIPULATION  ///////////////////////////////////////

Str str_copy(Str s, Arena *arena);
Str str_append(Str s1, Str suffix, Arena *arena);
Str str_prepend(Str s1, Str prefix, Arena *arena);
// Inserts sep in between elements
Str str_join(Str sep, usize count, Str s[count], Arena *arena);
// Appends suffix to every element
Str str_join_suffix(Str suffix, usize count, Str s[count], Arena *arena);
// Prepends prefix to every element
Str str_join_prefix(Str prefix, usize count, Str s[count], Arena *arena);
Str str_upper(Str s, Arena *arena);
Str str_lower(Str s, Arena *arena);
Str str_replace(Str s, Str old, Str new, Arena *arena);
Str str_center(Str s, usize width, char fillchar, Arena *arena);
Str str_ljust(Str s, usize width, char fillchar, Arena *arena);
Str str_rjust(Str s, usize width, char fillchar, Arena *arena);
Str str_repeat(Str s, usize count, Arena *arena);
Str str_reverse(Str s, Arena *arena);

// MANIPULATION END  ///////////////////////////////////////

// COMPARING  ///////////////////////////////////////

bool str_eq(Str s1, Str s2);
bool str_eq_ignorecase(Str s1, Str s2);
bool str_startswith(Str s1, Str prefix);
bool str_endswith(Str s1, Str suffix);

bool str_contains(Str haystack, Str needle);
bool str_includes(Str haystack, char needle);
bool str_empty(Str s);
CmpOrdering str_compare_gt(Str s1, Str s2);
CmpOrdering str_compare_lt(Str s1, Str s2);

// Returns function that you can pass to qsort
CompareFn str_compare_qsort(CmpOrdering ordering);

// COMPARING END ///////////////////////////////////////

// CHOPPING ///////////////////////////////////////

Str str_trim_left(Str s);
Str str_trim_right(Str s);
Str str_trim(Str s);

bool str_try_chop_by_delim(Str *s, char delim, Str *chunk);
Str str_chop_by_delim(Str *s, char delim);
bool str_try_chop_by_predicate(Str *s, bool (*predicate)(char), Str *chunk);
Str str_chop_by_predicate(Str *s, bool (*predicate)(char));
Str str_chop_right_by_delim(Str *s, char delim);
Str str_chop_right_by_predicate(Str *s, bool (*predicate)(char));
Str str_take(Str *s, usize count);
bool str_try_take(Str *s, usize n, Str *chunk);

Str str_substring(Str s, usize start, usize end);

// CHOPPING END ///////////////////////////////////////

// UTILS ///////////////////////////////////////

Str str_u64(Arena *arena, u64 n);
u64 str_to_u64(Str s);
u64 str_chop_u64(Str *s);

// Returns 'STR_NOT_FOUND' if 'needle' was not found.
usize str_find(Str haystack, Str needle);
// Returns 'STR_NOT_FOUND' if 'needle' was not found.
usize str_find_last(Str haystack, Str needle);
usize str_count(Str haystack, Str needle);
// Returns '\0' if the index is out of bounds.
char str_getc(Str s, usize idx);

// Basic FNV hash.
u64 str_hash(Str s);

// UTILS END ///////////////////////////////////////

#endif /* !__CLIB_STR_H__ */
