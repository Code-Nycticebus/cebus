/* DOCUMENTATION
## Features and Functions
- **String Creation and Printing**:
  - `STR("Hello World")`: Create a new string from a string literal.
  - `STR_STATIC("Hello")`: Create a new static string from a string literal.
  - `str_from_cstr(str)`: Create new string from a char array.
  - `str_from_bytes(str)`: Create new string from bytes.
  - `str_format(fmt, ...)`: Create new string as formated.
  - `printf(STR_FMT"\n", STR_ARG(str))`: Print strings using macros.

- **String Manipulation**:
  - `str_lower(str, &arena)`, `str_upper(str, &arena)`: Convert to lower or
upper case.
  - `str_append(str, suffix, &arena)`, `str_prepend(str, prefix, &arena)`:
Append or prepend strings.
  - `str_wrap(str, wrap, &arena)`: Wrap a string.
  - `str_join(sep, count, strs, &arena)`: Join strings with a separator.

- **String Trimming and Slicing**:
  - `str_trim(str)`, `str_trim_left(str)`, `str_trim_right(str)`: Trim
whitespace.
  - `str_chop_by_delim(str, delim)`, `str_try_chop_by_delim(str, delim,
&chunk)`: Chop strings by delimiter.
  - `str_substring(str, start, end)`: Extract a substring.

- **String Comparison and Search**:
  - `str_eq(s1, s2)`, `str_eq_ignorecase(s1, s2)`: Check string equality.
  - `str_startswith(str, prefix)`, `str_endswith(str, suffix)`: Check
prefixes/suffixes.
  - `str_contains(haystack, needle)`: Check if string contains a substring.

- **Conversion and Utility**:
  - `str_to_u64(str)`, `str_u64(n, &arena)`: Convert between strings and
unsigned 64-bit integers.
  - `str_hash(str)`: Generate a hash value for a string.

## Usage Example

```c
Arena arena = {0};
Str greeting = STR("Hello World");
Str lower = str_lower(greeting, &arena);
printf(STR_FMT"\n", STR_ARG(lower));
```
*/

#ifndef __CLIB_STR_H__
#define __CLIB_STR_H__

#include "clib/core/arena.h"
#include "clib/core/defines.h" // IWYU pragma: private: include "str.h"

///////////////////////////////////////////////////////////////////////////////

#define STR_NOT_FOUND SIZE_MAX

#define STR_LETTERS STR("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define STR_UPPERCASE STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define STR_LOWERCASE STR("abcdefghijklmnopqrstuvwxyz")
#define STR_DIGITS STR("0123456789")
#define STR_HEXDIGITS STR("0123456789abcdefABCDEF")
#define STR_PUNCTUATION STR("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~")
#define STR_WHITESPACE STR(" \t\n\r\x0b\x0c")

///////////////////////////////////////////////////////////////////////////////

Str str_from_parts(usize size, const char *cstr);
Str str_from_bytes(Bytes bytes);
Bytes str_to_bytes(Str s);
Str str_from_cstr(const char *cstr);
FMT(2) Str str_format(Arena *arena, const char *fmt, ...);

///////////////////////////////////////////////////////////////////////////////

Str str_copy(Str s, Arena *arena);
Str str_append(Str s1, Str suffix, Arena *arena);
Str str_prepend(Str s1, Str prefix, Arena *arena);
Str str_wrap(Str s, Str wrap, Arena *arena);

// Inserts sep in between elements
Str str_join(Str sep, usize count, Str *s, Arena *arena);
// Appends suffix to every element
Str str_join_suffix(Str suffix, usize count, Str *s, Arena *arena);
// Prepends prefix to every element
Str str_join_prefix(Str prefix, usize count, Str *s, Arena *arena);

Str str_join_wrap(Str sep, Str wrap, usize count, Str *s, Arena *arena);

Str str_upper(Str s, Arena *arena);
Str str_lower(Str s, Arena *arena);
Str str_replace(Str s, Str old, Str new, Arena *arena);
Str str_center(Str s, usize width, char fillchar, Arena *arena);
Str str_ljust(Str s, usize width, char fillchar, Arena *arena);
Str str_rjust(Str s, usize width, char fillchar, Arena *arena);
Str str_repeat(Str s, usize count, Arena *arena);
Str str_reverse(Str s, Arena *arena);

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

Str str_trim_left(Str s);
Str str_trim_left_by_delim(Str s, char delim);
Str str_trim_left_by_predicate(Str s, bool (*predicate)(char));

Str str_trim_right(Str s);
Str str_trim_right_by_delim(Str s, char delim);
Str str_trim_right_by_predicate(Str s, bool (*predicate)(char));

Str str_trim(Str s);
Str str_trim_by_delim(Str s, char delim);
Str str_trim_by_predicate(Str s, bool (*predicate)(char));

bool str_try_chop_by_delim(Str *s, char delim, Str *chunk);
Str str_chop_by_delim(Str *s, char delim);
bool str_try_chop_by_predicate(Str *s, bool (*predicate)(char), Str *chunk);
Str str_chop_by_predicate(Str *s, bool (*predicate)(char));
Str str_chop_right_by_delim(Str *s, char delim);
Str str_chop_right_by_predicate(Str *s, bool (*predicate)(char));
Str str_take(Str *s, usize count);
bool str_try_take(Str *s, usize n, Str *chunk);

Str str_substring(Str s, usize start, usize end);

///////////////////////////////////////////////////////////////////////////////

u64 str_u64(Str s);
u64 str_chop_u64(Str *s);

i64 str_i64(Str s);
i64 str_chop_i64(Str *s);

f64 str_f64(Str s);
f64 str_chop_f64(Str *s);

///////////////////////////////////////////////////////////////////////////////

// Returns 'STR_NOT_FOUND' if 'needle' was not found.
usize str_find(Str haystack, Str needle);
// Returns 'STR_NOT_FOUND' if 'needle' was not found.
usize str_find_last(Str haystack, Str needle);
usize str_count(Str haystack, Str needle);
// Returns '\0' if the index is out of bounds.
char str_getc(Str s, usize idx);

// Basic FNV hash.
u64 str_hash(Str s);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_STR_H__ */
