#ifndef __CLIB_ASSERTS_H__
#define __CLIB_ASSERTS_H__

/* DOCUMENTATION
## Usage

Use the assertion macros to validate conditions and log failures:

```c
clib_assert(1 == 1, "It needs a message");
clib_assert(2 == 2, "It can even handle arguments: %d", 420);
```

## Available Macros

- `clib_assert(EXPR, FMT, ...)`: Asserts a condition and logs an error if the
condition is false.
- `clib_assert_warn(EXPR, FMT, ...)`: Asserts a condition and logs a warning if
the condition is false.
- `clib_assert_debug(EXPR, FMT, ...)`: Debug-only assert that logs a debug
message if the condition is false.
- `clib_assert_return(EXPR, RETURN_VALUE)`: Asserts a condition and returns a
value if the condition is false.

## Debug Break

A `debugbreak` is triggered on assertion failure to facilitate debugging, with
behavior varying by platform:

- **Linux**: Uses `raise(SIGTRAP)`.
- **Windows**: Uses `__debugbreak()`.
- **Other Platforms**: Uses `abort()`.
*/

#include "clib/core/platform.h" // IWYU pragma: export
#include "logging.h"            // IWYU pragma: export

////////////////////////////////////////////////////////////////////////////

#if defined(LINUX)
#include <signal.h> // IWYU pragma: keep
#define DEBUGBREAK() raise(SIGTRAP)
#elif defined(WINDOWS)
#include <intrin.h> // IWYU pragma: export
#define DEBUGBREAK() __debugbreak();
#else
#include <stdlib.h> // IWYU pragma: export
#define DEBUGBREAK(...) abort()
#endif

////////////////////////////////////////////////////////////////////////////

#if defined(NDEBUG)
#if defined(GCC) || defined(CLANG) || defined(MINGW32) || defined(MINGW64)
#define UNREACHABLE() __builtin_unreachable()
#elif defined(MSVC)
#define UNREACHABLE() __assume(0)
#else
#define UNREACHABLE()
#endif
#else
#include <stdlib.h> // IWYU pragma: export
#define UNREACHABLE()                                                          \
  clib_log_error("UNREACHABLE: %s:%d: %s()", __FILE__, __LINE__, __func__);    \
  abort()
#endif

////////////////////////////////////////////////////////////////////////////

#define _clib_assert_print(level, expr, ...)                                   \
  clib_log_level(level, "%s:%d: %s():", __FILE__, __LINE__, __func__);         \
  clib_log_level(level, "  Assertion '%s' failed", expr);                      \
  clib_log_level(level, "  Description: "__VA_ARGS__)

#define clib_assert(expression, ...)                                           \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print(CLIB_LOG_ERROR, #expression, __VA_ARGS__);            \
      DEBUGBREAK();                                                            \
    }                                                                          \
  } while (0)

#define clib_assert_return(expression, ret)                                    \
  do {                                                                         \
    if (!(expression)) {                                                       \
      return ret;                                                              \
    }                                                                          \
  } while (0)

#define clib_assert_warn(expression, ...)                                      \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print(CLIB_LOG_WARNING, #expression, __VA_ARGS__);          \
    }                                                                          \
  } while (0)

#ifndef NDEBUG
#define clib_assert_debug(expression, ...)                                     \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print(CLIB_LOG_DEBUG, #expression, __VA_ARGS__);            \
      DEBUGBREAK();                                                            \
    }                                                                          \
  } while (0)
#else
#define clib_assert_debug(...)
#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_ASSERTS_H__ */
