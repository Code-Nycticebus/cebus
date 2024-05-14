/* DOCUMENTATION
## Usage

Call the function with the message format and arguments:

```c
cebus_log_error("This is an error message: %s", error_details);
```

## Functions

- `cebus_log_level(log_level, fmt, ...)`: Logs a message with the specified log
level.
- `cebus_log(fmt, ...)`: Logs a message.
- `cebus_log_fatal(fmt, ...)`: Logs a fatal message.
- `cebus_log_error(fmt, ...)`: Logs an error message.
- `cebus_log_warning(fmt, ...)`: Logs a warning message.
- `cebus_log_info(fmt, ...)`: Logs an info message.
- `cebus_log_debug(fmt, ...)`: Logs a debug message (only in debug builds).
- `cebus_log_trace(fmt, ...)`: Logs a trace message (only in debug builds).
*/

#ifndef __CEBUS_LOGGING_H__
#define __CEBUS_LOGGING_H__

#include "cebus/core/defines.h"

////////////////////////////////////////////////////////////////////////////

typedef enum {
  CEBUS_LOG = 0,
  CEBUS_LOG_FATAL,
  CEBUS_LOG_ERROR,
  CEBUS_LOG_WARNING,
  CEBUS_LOG_INFO,
  CEBUS_LOG_DEBUG,
  CEBUS_LOG_TRACE,
} LogLevel;

////////////////////////////////////////////////////////////////////////////

void FMT(2) cebus_log_level(LogLevel log_level, const char *fmt, ...);
void FMT(1) cebus_log(const char *fmt, ...);
void FMT(1) cebus_log_fatal(const char *fmt, ...);
void FMT(1) cebus_log_error(const char *fmt, ...);
void FMT(1) cebus_log_warning(const char *fmt, ...);
void FMT(1) cebus_log_info(const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////

void FMT(1) _cebus_log_debug(const char *fmt, ...);
void FMT(1) _cebus_log_trace(const char *fmt, ...);

#ifndef NDEBUG
#define cebus_log_debug(...) _cebus_log_debug(__VA_ARGS__)
#define cebus_log_trace(...) _cebus_log_trace(__VA_ARGS__)
#else
#define cebus_log_debug(...)
#define cebus_log_trace(...)
#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_LOGGING_H__ */
