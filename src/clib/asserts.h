#ifndef __CLIB_ASSERTS_H__
#define __CLIB_ASSERTS_H__

#include "defines.h"
#include "logging.h"
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

#define _clib_assert_print(level, msg, expression)                             \
  clib_log(level, "%s:%d: assert failed: '%s': %s", __FILE__, __LINE__,        \
           expression, msg);

#define clib_assert(expression, msg)                                           \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print(CLIB_LOG_ERROR, msg, #expression);                    \
      debugbreak();                                                            \
    }                                                                          \
  } while (0)

#define clib_assert_warn(expression, msg)                                      \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print(CLIB_LOG_WARNING, msg, #expression);                  \
    }                                                                          \
  } while (0)

#ifndef NDEBUG
#define clib_assert_debug(expression, msg)                                     \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print(CLIB_LOG_ERROR, msg, #expression);                    \
      debugbreak();                                                            \
    }                                                                          \
  } while (0)
#else
#define clib_assert_debug(...)
#endif

#endif /* ifndef __CLIB_ASSERTS_H__ */
