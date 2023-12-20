#ifndef __CLIB_ASSERTS_H__
#define __CLIB_ASSERTS_H__

#include "defines.h"
#include <stdlib.h>

#if defined(__GNUC__) || defined(__clang__)
#include <signal.h>
#define debugbreak() raise(SIGTRAP)
#elif defined(_MSC_VER)
#include <intrin.h>
#define debugbreak() __debugbreak()
#else
#define debugbreak(...) abort()
#endif

void _clib_assert_print_error(const char *msg, const char *expression,
                              const char *filename, int line);
void _clib_assert_print_warning(const char *msg, const char *expression,
                                const char *filename, int line);

#define clib_assert(expression, msg)                                           \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print_error(msg, #expression, __FILE__, __LINE__);          \
      debugbreak();                                                            \
    }                                                                          \
  } while (0)

#define clib_assert_warn(expression, msg)                                      \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print_warning(msg, #expression, __FILE__, __LINE__);        \
    }                                                                          \
  } while (0)

#ifndef NDEBUG
#define clib_assert_debug(expression, msg)                                     \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print_error(msg, #expression, __FILE__, __LINE__);          \
      debugbreak();                                                            \
    }                                                                          \
  } while (0)
#else
#define clib_assert_debug(...)
#endif

#endif /* ifndef __CLIB_ASSERTS_H__ */
