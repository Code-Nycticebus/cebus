#ifndef __CLIB_CHAR_H__
#define __CLIB_CHAR_H__

#include "core/defines.h"

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
