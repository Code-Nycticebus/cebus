#ifndef __CLIB_CHAR_H__
#define __CLIB_CHAR_H__

#include "core/defines.h"

///////////////////////////////////////////////////////////////////////////////

bool CONST c_is_alnum(char c);
bool CONST c_is_alpha(char c);
bool CONST c_is_lower(char c);
bool CONST c_is_upper(char c);
bool CONST c_is_space(char c);
bool CONST c_is_cntrl(char c);
bool CONST c_is_print(char c);
bool CONST c_is_graph(char c);
bool CONST c_is_blank(char c);
bool CONST c_is_punct(char c);
bool CONST c_is_digit(char c);
bool CONST c_is_xdigit(char c);

///////////////////////////////////////////////////////////////////////////////

char CONST c_to_lower(char c);
char CONST c_to_upper(char c);

u8 CONST c_to_u8(char c);
u8 CONST c_hex_to_u8(char c);

char CONST c_u8_to_c(u8 d);
char CONST c_u8_to_hex(u8 d);
char CONST c_u8_to_HEX(u8 d);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_CHAR_H__ */
