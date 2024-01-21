#ifndef __CLIB_CHAR_H__
#define __CLIB_CHAR_H__

/* DOCUMENTATION

Just a wrapper around ```#include <ctype.h>```

```c_is_alnum();```
checks for an alphanumeric character; it is equivalent to (isalpha(c) ||
isdigit(c)).

```c_is_alpha();```
checks for an alphabetic character; in the standard "C" locale, it is
equivalent to (isupper(c) || islower(c)). In some locales, there may be
additional characters for which isalpha() is true-letters which are neither
upper case nor lower case.

```c_is_ascii();```
checks whether c is a 7-bit unsigned char value that fits into the ASCII
character set.

```c_is_blank();```
checks for a blank character; that is, a space or a tab.

```c_is_cntrl();```
checks for a control character.

```c_is_digit();```
checks for a digit (0 through 9).

```c_is_graph();```
checks for any printable character except space.

```c_is_lower();```
checks for a lower-case character.

```c_is_print();```
checks for any printable character including space.

```c_is_punct();```
checks for any printable character which is not a space or an alphanumeric
character. isspace() checks for white-space characters. In the "C" and "POSIX"
locales, these are: space, form-feed ('\f'), newline ('\n'), carriage return
('\r'), horizontal tab ('\t'), and vertical tab ('\v').

```c_is_upper();```
checks for an uppercase letter.

```c_is_xdigit();```
checks for a hexadecimal digits, that is, one of 0 1 2 3 4 5 6 7 8 9 a b c d e f
A B C D E F.

*/

#include "core/defines.h"

///////////////////////////////////////////////////////////////////////////////

bool c_is_alnum(char c);
bool c_is_alpha(char c);
bool c_is_lower(char c);
bool c_is_upper(char c);
bool c_is_space(char c);
bool c_is_cntrl(char c);
bool c_is_print(char c);
bool c_is_graph(char c);
bool c_is_blank(char c);
bool c_is_punct(char c);
bool c_is_digit(char c);
bool c_is_xdigit(char c);

///////////////////////////////////////////////////////////////////////////////

char c_to_lower(char c);
char c_to_upper(char c);
u8 c_to_digit(char c);
u8 c_to_xdigit(char c);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_CHAR_H__ */
