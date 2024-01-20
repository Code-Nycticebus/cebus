#ifndef __CLIB_STR_H__
#define __CLIB_STR_H__

#include "core/arena.h"

#include "core/defines.h" // IWYU pragma: private: include "str.h"

#define STR_NOT_FOUND SIZE_MAX

#define STR_LETTERS STR("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define STR_UPPERCASE STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define STR_LOWERCASE STR("abcdefghijklmnopqrstuvwxyz")
#define STR_DIGITS STR("0123456789")
#define STR_HEXDIGITS STR("0123456789abcdefABCDEF")
#define STR_PUNCTUATION STR("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~")
#define STR_WHITESPACE STR(" \t\n\r\x0b\x0c")

/**********************
 * String Constructors
 **********************/

/*
 * Constructs Str from a char array and its size
 */
Str str_from_parts(usize size, const char *cstr);

/*
 * Converts a Str from a Bytes type
 */
Str str_from_bytes(Bytes bytes);

/*
 * Converts a Str to the Bytes type
 */
Bytes str_to_bytes(Str s);

/*
 * Constructs a Str from a null terminated char array
 */
Str str_from_cstr(const char *cstr);

/**********************
 * String Manipulation
 **********************/

/*
 * Gets a char of the specified index.
 * Returns '\0' if the index is out of bounds.
 */
char str_getc(Str s, usize idx);

/*
 * Returns a new exact copy of a string
 */
Str str_copy(Str s, Arena *arena);

/*
 * Returns a new string with a suffix
 */
Str str_append(Str s1, Str suffix, Arena *arena);

/*
 * Returns a new string with a prefix
 */
Str str_prepend(Str s1, Str prefix, Arena *arena);

/*
 *  Joins strings together with a seperator in between every element.
 */
Str str_join(Str sep, usize count, Str s[count], Arena *arena);

/*
 *  Joins strings together with a seperater in after every element.
 */
Str str_join_suffix(Str sep, usize count, Str s[count], Arena *arena);

/*
 * Returns a new uppercase string
 */
Str str_upper(Str s, Arena *arena);

/*
 * Returns a new lowercase string
 */
Str str_lower(Str s, Arena *arena);

/*
 * Returns a new string replaceing all occurences of 'old' with 'new'
 */
Str str_replace(Str s, Str old, Str new, Arena *arena);

/*
 * Return centered in a new string of length 'width'.
 * Padding is done using the specified 'fillchar'
 */
Str str_center(Str s, usize width, char fillchar, Arena *arena);

/*
 * Return a new string left justified in a string of length 'width'.
 * Padding is done using the specified 'fillchar'
 */
Str str_ljust(Str s, usize width, char fillchar, Arena *arena);

/*
 * Return a new string right justified in a string of length 'width'.
 * Padding is done using the specified 'fillchar'
 */
Str str_rjust(Str s, usize width, char fillchar, Arena *arena);

/*
 * Return a new string that repeats 'count' times.
 */
Str str_repeat(Str s, usize count, Arena *arena);

/*
 * Return a new reversed string
 */
Str str_reverse(Str s, Arena *arena);

/**********************
 * String Comparing
 **********************/

/*
 * Tests if strings are equal
 */
bool str_eq(Str s1, Str s2);

/*
 * Tests if strings are equal while ignoring case
 */
bool str_eq_ignorecase(Str s1, Str s2);

/*
 * Tests if string starts with 'prefix'
 */
bool str_startswith(Str s1, Str prefix);

/*
 * Tests if string ends with 'suffix'
 */
bool str_endswith(Str s1, Str suffix);

/*
 * Tests if string contains a string
 */
bool str_contains(Str haystack, Str needle);

/*
 * Tests if string contains a char
 */
bool str_includes(Str haystack, char needle);

/*
 * Tests if string is empty
 */
bool str_empty(Str s);

/*
 * Function to compare strings returning a enum CmpOrdering
 */
CmpOrdering str_compare_gt(Str s1, Str s2);

/*
 * Function to compare strings returning a enum CmpOrdering
 */
CmpOrdering str_compare_lt(Str s1, Str s2);

/*
 * Function that returns function that you can pass to qsort
 */
CompareFn str_compare_qsort(CmpOrdering ordering);

/**********************
 * String Chopping
 **********************/

/*
 * Trims any whitespace left of string
 */
Str str_trim_left(Str s);

/*
 * Trims any whitespace right of string
 */
Str str_trim_right(Str s);

/*
 * Trims any whitespace left and right of string
 */
Str str_trim(Str s);

/*
 * Chops by delimiter.
 * Returns false if it can't chop anymore.
 */
bool str_try_chop_by_delim(Str *s, char delim, Str *chunk);

/*
 * Chops by delimiter.
 */
Str str_chop_by_delim(Str *s, char delim);

/*
 * Chops by function.
 * Returns false if it can't chop anymore.
 */
bool str_try_chop_by_predicate(Str *s, bool (*predicate)(char), Str *chunk);

/*
 * Chops by function.
 */
Str str_chop_by_predicate(Str *s, bool (*predicate)(char));

/*
 * Chops by delimeter starting from the right.
 */
Str str_chop_right_by_delim(Str *s, char delim);

/*
 * Chops by function starting from the right.
 */
Str str_chop_right_by_predicate(Str *s, bool (*predicate)(char));

/*
 * Takes 'count' amount of chars.
 */
Str str_take(Str *s, usize count);

/*
 * Tries to take 'count' amount of chars.
 * Returns false if it can't take anymore.
 */
bool str_try_take(Str *s, usize n, Str *chunk);

/*
 * Returns substring of string
 */
Str str_substring(Str s, usize start, usize end);

/**********************
 * String Utils
 **********************/

/*
 * Converts u64 to string.
 */
Str str_u64(Arena *arena, u64 n);

/*
 * Converts string to u64.
 * TODO error handling
 */
u64 str_to_u64(Str s);

/*
 * Chops of parsed u64 from string
 */
u64 str_chop_u64(Str *s);

/*
 * Finds index of 'needle' in string.
 * Returns 'STR_NOT_FOUND' if 'needle' was not found.
 */
usize str_find(Str haystack, Str needle);

/*
 * Finds index of 'needle' in string starting from the right
 * Returns 'STR_NOT_FOUND' if 'needle' was not found.
 */
usize str_find_last(Str haystack, Str needle);

/*
 * Counts occurences of 'needle' in string.
 */
usize str_count(Str haystack, Str needle);

/*
 * Basic FNV hash.
 */
u64 str_hash(Str s);

#endif /* !__CLIB_STR_H__ */
