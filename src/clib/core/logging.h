#ifndef __CLIB_LOGGING_H__
#define __CLIB_LOGGING_H__

#include "clib/core/defines.h"

////////////////////////////////////////////////////////////////////////////

typedef enum {
  CLIB_LOG_FATAL = 0,
  CLIB_LOG_ERROR,
  CLIB_LOG_WARNING,
  CLIB_LOG_INFO,
  CLIB_LOG_DEBUG,
  CLIB_LOG_TRACE,
} LogLevel;

////////////////////////////////////////////////////////////////////////////

void FMT(2) clib_log(LogLevel log_level, const char *fmt, ...);
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
