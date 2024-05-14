#ifndef __CEBUS_ASSERTS_H__
#define __CEBUS_ASSERTS_H__

/* DOCUMENTATION
## Usage

Use the assertion macros to validate conditions and log failures:

```c
cebus_assert(1 == 1, "It needs a message");
cebus_assert(2 == 2, "It can even handle arguments: %d", 420);
```

## Assert Macros

- `cebus_assert(EXPR, FMT, ...)`: Asserts a condition and logs an error if the
condition is false.
- `cebus_assert_warn(EXPR, FMT, ...)`: Asserts a condition and logs a warning if
the condition is false.
- `cebus_assert_debug(EXPR, FMT, ...)`: Debug-only assert that logs a debug
message if the condition is false.
- `cebus_assert_return(EXPR, RETURN_VALUE)`: Asserts a condition and returns a
value if the condition is false.

## Debug Macros

- `DEBUGBREAK()`: Uses platform specific debug break.
- `UNREACHABLE()`: Prints error message in debug mode. In release mode it uses
compiler intrensics.
- `NOT_IMPLEMENTED()`: Prints error message
*/

#include "cebus/core/platform.h" // IWYU pragma: export
#include "logging.h"             // IWYU pragma: export

NORETURN void abort(void);

////////////////////////////////////////////////////////////////////////////

#if defined(LINUX)
#include <signal.h> // IWYU pragma: keep
#define DEBUGBREAK() raise(SIGTRAP)
#elif defined(WINDOWS)
#include <intrin.h> // IWYU pragma: export
#define DEBUGBREAK() __debugbreak()
#else
#define DEBUGBREAK(...) abort()
#endif

////////////////////////////////////////////////////////////////////////////

#if defined(NDEBUG)
#if defined(GCC) || defined(CLANG) || defined(MINGW32) || defined(MINGW64)
#define UNREACHABLE()                                                          \
  __builtin_unreachable();                                                     \
  abort()
#elif defined(MSVC)
#define UNREACHABLE()                                                          \
  __assume(0);                                                                 \
  abort()
#else
#define UNREACHABLE() abort()
#endif
#else
#define UNREACHABLE()                                                          \
  cebus_log_error("UNREACHABLE: %s:%d: %s()", __FILE__, __LINE__, __func__);   \
  abort()
#endif

////////////////////////////////////////////////////////////////////////////

#if defined(NDEBUG)
#define NOT_IMPLEMENTED() abort()
#else
#define NOT_IMPLEMENTED()                                                      \
  cebus_log_error("NOT IMPLEMENTED: %s:%d: %s()", __FILE__, __LINE__,          \
                  __func__);                                                   \
  abort()
#endif

////////////////////////////////////////////////////////////////////////////

#define _cebus_assert_print(level, expr, ...)                                  \
  cebus_log_level(level, "%s:%d: %s():", __FILE__, __LINE__, __func__);        \
  cebus_log_level(level, "  Assertion '%s' failed", expr);                     \
  cebus_log_level(level, "  Description: "__VA_ARGS__)

#define cebus_assert(expression, ...)                                          \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _cebus_assert_print(CEBUS_LOG_ERROR, #expression, __VA_ARGS__);          \
      DEBUGBREAK();                                                            \
    }                                                                          \
  } while (0)

#define cebus_assert_return(expression, ret)                                   \
  do {                                                                         \
    if (!(expression)) {                                                       \
      return ret;                                                              \
    }                                                                          \
  } while (0)

#define cebus_assert_warn(expression, ...)                                     \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _cebus_assert_print(CEBUS_LOG_WARNING, #expression, __VA_ARGS__);        \
    }                                                                          \
  } while (0)

#ifndef NDEBUG
#define cebus_assert_debug(expression, ...)                                    \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _cebus_assert_print(CEBUS_LOG_DEBUG, #expression, __VA_ARGS__);          \
      DEBUGBREAK();                                                            \
    }                                                                          \
  } while (0)
#else
#define cebus_assert_debug(...)
#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_ASSERTS_H__ */
