#include "char.h"
#include <ctype.h>

///////////////////////////////////////////////////////////////////////////////

bool c_is_alpha(char c) { return isalpha(c); }
bool c_is_space(char c) { return isspace(c); }
bool c_is_digit(char c) { return isdigit(c); }
bool c_is_xdigit(char c) { return isxdigit(c); }

///////////////////////////////////////////////////////////////////////////////

char c_to_lower(char c) { return (char)tolower(c); }
char c_to_upper(char c) { return (char)toupper(c); }
u8 c_to_digit(char c) { return '0' <= c && c <= '9' ? (u8)(c - '0') : 0; }
u8 c_to_xdigit(char c) {
  if ('0' <= c && c <= '9')
    return (u8)c - '0';
  if ('a' <= c && c <= 'f')
    return 10 + ((u8)c - 'a');
  if ('A' <= c && c <= 'F')
    return 10 + ((u8)c - 'A');
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
