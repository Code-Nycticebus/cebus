#ifndef __CLIB_LOGGING_H__
#define __CLIB_LOGGING_H__

#include "core/platform.h"

#include <stdio.h> // IWYU pragma: export

typedef enum {
  CLIB_LOG_FATAL = 0,
  CLIB_LOG_ERROR,
  CLIB_LOG_WARNING,
  CLIB_LOG_INFO,
  CLIB_LOG_DEBUG,
  CLIB_LOG_TRACE,
} LogLevel;

void clib_log(FILE *f, LogLevel log_level, const char *fmt, ...) CLIB_FMT(3, 4);
void clib_log_fatal(const char *fmt, ...) CLIB_FMT(1, 2);
void clib_log_error(const char *fmt, ...) CLIB_FMT(1, 2);
void clib_log_warning(const char *fmt, ...) CLIB_FMT(1, 2);
void clib_log_info(const char *fmt, ...) CLIB_FMT(1, 2);

#ifndef NDEBUG
void clib_log_debug(const char *fmt, ...) CLIB_FMT(1, 2);
void clib_log_trace(const char *fmt, ...) CLIB_FMT(1, 2);
#else
#define clib_log_debug(...)
#define clib_log_trace(...)
#endif

#endif /* !__CLIB_LOGGING_H__ */
