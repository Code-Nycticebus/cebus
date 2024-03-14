/* DOCUMENTATION
## Character Classification Functions

- `c_is_alnum(c)`: Checks if a character is alphanumeric.
- `c_is_alpha(c)`: Checks if a character is alphabetic.
- `c_is_lower(c)`: Checks if a character is lowercase.
- `c_is_upper(c)`: Checks if a character is uppercase.
- `c_is_space(c)`: Checks if a character is a whitespace character.
- `c_is_cntrl(c)`: Checks if a character is a control character.
- `c_is_print(c)`: Checks if a character is printable.
- `c_is_graph(c)`: Checks if a character has a graphical representation.
- `c_is_blank(c)`: Checks if a character is blank (space or tab).
- `c_is_punct(c)`: Checks if a character is punctuation.
- `c_is_digit(c)`: Checks if a character is a digit.
- `c_is_xdigit(c)`: Checks if a character is a hexadecimal digit.

## Character Conversion Functions

- `c_to_lower(c)`: Converts a character to lowercase.
- `c_to_upper(c)`: Converts a character to uppercase.
- `c_to_u8(c)`: Converts a character to an unsigned 8-bit integer.
- `c_hex_to_u8(c)`: Converts a hexadecimal character to an unsigned 8-bit
integer.
- `c_u8_to_c(d)`: Converts an unsigned 8-bit integer to a character.
- `c_u8_to_hex(d)`: Converts an unsigned 8-bit integer to a hexadecimal
character (lowercase).
- `c_u8_to_HEX(d)`: Converts an unsigned 8-bit integer to a hexadecimal
character (uppercase).
*/

#ifndef __CLIB_CHAR_H__
#define __CLIB_CHAR_H__

#include "clib/core/defines.h"

///////////////////////////////////////////////////////////////////////////////

CONST_FN bool c_is_alnum(char c);
CONST_FN bool c_is_alpha(char c);
CONST_FN bool c_is_lower(char c);
CONST_FN bool c_is_upper(char c);
CONST_FN bool c_is_space(char c);
CONST_FN bool c_is_cntrl(char c);
CONST_FN bool c_is_print(char c);
CONST_FN bool c_is_graph(char c);
CONST_FN bool c_is_blank(char c);
CONST_FN bool c_is_punct(char c);
CONST_FN bool c_is_digit(char c);
CONST_FN bool c_is_xdigit(char c);

///////////////////////////////////////////////////////////////////////////////

CONST_FN char c_to_lower(char c);
CONST_FN char c_to_upper(char c);

CONST_FN u8 c_to_u8(char c);
CONST_FN u8 c_hex_to_u8(char c);

CONST_FN char c_u8_to_c(u8 d);
CONST_FN char c_u8_to_hex(u8 d);
CONST_FN char c_u8_to_HEX(u8 d);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_CHAR_H__ */
