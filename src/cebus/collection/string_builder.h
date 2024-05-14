/* DOCUMENTATION
The `StringBuilder` provides functionality for efficiently constructing
strings.

> :warning: StringBuilder does not construct '\0' terminated strings.

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

- **`void sb_append_va(StringBuilder *sb, const char *fmt, va_list va);`**
  Appends a formatted string and va_list to the `StringBuilder`, similar to
`vprintf` style formatting.

*/

#ifndef __CEBUS_STRING_BUILDER_H__
#define __CEBUS_STRING_BUILDER_H__

#include "cebus/core/defines.h"

#include "cebus/collection/da.h"
#include "cebus/core/arena.h"

#include <stdarg.h>

typedef DA(char) StringBuilder;

StringBuilder sb_init(Arena *arena);
void sb_clear(StringBuilder *sb);

Str sb_to_str(StringBuilder *sb);

void sb_append_parts(StringBuilder *sb, usize size, const char *s);
void sb_append_cstr(StringBuilder *sb, const char *cstr);
void sb_append_str(StringBuilder *sb, Str str);
void sb_append_c(StringBuilder *sb, char c);
FMT(2) usize sb_append_fmt(StringBuilder *sb, const char *fmt, ...);
usize sb_append_va(StringBuilder *sb, const char *fmt, va_list va);

#endif /* !__CEBUS_STRING_BUILDER_H__ */
