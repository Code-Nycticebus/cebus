/* DOCUMENTATION
The `StringBuilder` provides functionality for efficiently constructing
strings.

## Data Structures

- **StringBuilder**: A structure holding the dynamic array used as the buffer
for constructing strings.

## Functions

- **`StringBuilder sb_init(Arena *arena);`**
  Initializes a new `StringBuilder` instance, allocating its buffer using the
provided memory `arena`.

- **`Str sb_to_str(StringBuilder *sb);`**
  Converts the contents of the `StringBuilder` to a `Str`, effectively
finalizing the string construction.

- **`void sb_append_parts(StringBuilder *sb, usize size, const char *s);`**
  Appends parts of a string to the `StringBuilder`, where `size` specifies the
number of characters to append, and `s` points to the string parts to be
appended.

- **`void sb_append_cstr(StringBuilder *sb, const char *cstr);`**
  Appends a C-style null-terminated string to the `StringBuilder`.

- **`void sb_append_str(StringBuilder *sb, Str str);`**
  Appends a `Str` type string to the `StringBuilder`.

- **`void sb_append_fmt(StringBuilder *sb, const char *fmt, ...);`**
  Appends a formatted string to the `StringBuilder`, similar to `printf` style
formatting.

*/

#ifndef __CLIB_STRING_BUILDER_H__
#define __CLIB_STRING_BUILDER_H__

#include "clib/core/defines.h"

#include "clib/collection/da.h"
#include "clib/core/arena.h"

typedef struct {
  DA(char) buffer;
} StringBuilder;

StringBuilder sb_init(Arena *arena);

Str sb_to_str(StringBuilder *sb);

void sb_append_parts(StringBuilder *sb, usize size, const char *s);
void sb_append_cstr(StringBuilder *sb, const char *cstr);
void sb_append_str(StringBuilder *sb, Str str);
void sb_append_fmt(StringBuilder *sb, const char *fmt, ...);

#endif /* !__CLIB_STRING_BUILDER_H__ */
