/* DOCUMENTATION
## Usage

Call the function with the message format and arguments:

```c
clib_log_error("This is an error message: %s", error_details);
```

## Functions

- `clib_log_level(log_level, fmt, ...)`: Logs a message with the specified log
level.
- `clib_log(fmt, ...)`: Logs a message.
- `clib_log_fatal(fmt, ...)`: Logs a fatal message.
- `clib_log_error(fmt, ...)`: Logs an error message.
- `clib_log_warning(fmt, ...)`: Logs a warning message.
- `clib_log_info(fmt, ...)`: Logs an info message.
- `clib_log_debug(fmt, ...)`: Logs a debug message (only in debug builds).
- `clib_log_trace(fmt, ...)`: Logs a trace message (only in debug builds).
*/

#ifndef __CLIB_LOGGING_H__
#define __CLIB_LOGGING_H__

#include "clib/core/defines.h"

////////////////////////////////////////////////////////////////////////////

typedef enum {
  CLIB_LOG = 0,
  CLIB_LOG_FATAL,
  CLIB_LOG_ERROR,
  CLIB_LOG_WARNING,
  CLIB_LOG_INFO,
  CLIB_LOG_DEBUG,
  CLIB_LOG_TRACE,
} LogLevel;

////////////////////////////////////////////////////////////////////////////

void FMT(2) clib_log_level(LogLevel log_level, const char *fmt, ...);
void FMT(1) clib_log(const char *fmt, ...);
void FMT(1) clib_log_fatal(const char *fmt, ...);
void FMT(1) clib_log_error(const char *fmt, ...);
void FMT(1) clib_log_warning(const char *fmt, ...);
void FMT(1) clib_log_info(const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////

void FMT(1) _clib_log_debug(const char *fmt, ...);
void FMT(1) _clib_log_trace(const char *fmt, ...);

#ifndef NDEBUG
#define clib_log_debug(...) _clib_log_debug(__VA_ARGS__)
#define clib_log_trace(...) _clib_log_trace(__VA_ARGS__)
#else
#define clib_log_debug(...)
#define clib_log_trace(...)
#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_LOGGING_H__ */
