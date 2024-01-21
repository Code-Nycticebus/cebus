#include "char.h"
#include "core/asserts.h"

#include <ctype.h>

///////////////////////////////////////////////////////////////////////////////

bool c_is_alnum(char c) { return isalnum(c); }
bool c_is_alpha(char c) { return isalpha(c); }
bool c_is_lower(char c) { return islower(c); }
bool c_is_upper(char c) { return isupper(c); }
bool c_is_space(char c) { return isspace(c); }
bool c_is_cntrl(char c) { return iscntrl(c); }
bool c_is_print(char c) { return isprint(c); }
bool c_is_graph(char c) { return isgraph(c); }
bool c_is_blank(char c) { return isblank(c); }
bool c_is_punct(char c) { return ispunct(c); }
bool c_is_digit(char c) { return isdigit(c); }
bool c_is_xdigit(char c) { return isxdigit(c); }

///////////////////////////////////////////////////////////////////////////////

char c_to_lower(char c) { return (char)tolower(c); }
char c_to_upper(char c) { return (char)toupper(c); }

u8 c_to_u8(char c) {
  clib_assert_debug(c_is_digit(c), "not convertible: '%c'", c);
  return (u8)c - '0';
}

u8 c_hex_to_u8(char c) {
  clib_assert_debug(c_is_xdigit(c), "not convertible: '%c'", c);
  if ('0' <= c && c <= '9')
    return c_to_u8(c);
  if ('a' <= c && c <= 'f')
    return 10 + (u8)c - 'a';
  if ('A' <= c && c <= 'F')
    return 10 + (u8)c - 'A';
  return 0;
}

char c_u8_to_c(u8 d) {
  clib_assert_debug(d < 10, "not convertible: '%d'", d);
  return '0' + (i8)d;
}

char c_u8_to_hex(u8 d) {
  clib_assert_debug(d < 16, "not convertible: '%d'", d);
  if (d < 10) {
    return c_u8_to_c(d);
  } else if (d < 15) {
    return 'a' + ((i8)d - 10);
  }
  return 0;
}

char c_u8_to_HEX(u8 d) {
  clib_assert_debug(d < 16, "not convertible: '%d'", d);
  if (d < 10) {
    return c_u8_to_c(d);
  }
  if (d < 16) {
    return 'A' + ((i8)d - 10);
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
