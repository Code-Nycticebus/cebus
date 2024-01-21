#ifndef __CLIB_CHAR_H__
#define __CLIB_CHAR_H__

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
