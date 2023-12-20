
#include "logging.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#define CLIB_LOGGER_CHAR_BUFFER_SIZE 1000

struct CmLogLevelPrefix {
  const char *prefix;
  const char *color;
};

typedef enum {
  CLIB_LOG_FATAL = 0,
  CLIB_LOG_ERROR,
  CLIB_LOG_WARNING,
  CLIB_LOG_INFO,
  CLIB_LOG_DEBUG,
  CLIB_LOG_TRACE,
} CmLogLevel;

#define FMT_RESET "\033[0m"

#define _LOG(__log_level, __fmt)                                               \
  char __buffer[CLIB_LOGGER_CHAR_BUFFER_SIZE];                                 \
  va_list __args;                                                              \
  va_start(__args, __fmt);                                                     \
  vsnprintf(__buffer, CLIB_LOGGER_CHAR_BUFFER_SIZE, __fmt, __args);            \
  va_end(__args);                                                              \
  clib_log(__log_level, __buffer);

static void clib_log(CmLogLevel log_level, const char *msg) {
  static bool display_colors = false;
  static bool tty_determent = false;
  if (!tty_determent) {
    tty_determent = true;
    display_colors = isatty(fileno(stdout));
  }
  static const struct CmLogLevelPrefix log_level_str[] = {
      [CLIB_LOG_FATAL] = {"FATAL", "\033[1m\033[91m"},
      [CLIB_LOG_ERROR] = {"ERROR", "\033[91m"},
      [CLIB_LOG_WARNING] = {"WARNING", "\033[33m"},
      [CLIB_LOG_INFO] = {"INFO", "\033[34m"},
      [CLIB_LOG_DEBUG] = {"DEBUG", "\033[1m\033[95m"},
      [CLIB_LOG_TRACE] = {"TRACE", "\033[97m"},
  };

  fprintf(stdout, "%s[%s]: %s%s\n",
          display_colors ? log_level_str[log_level].color : "",
          log_level_str[log_level].prefix, msg,
          display_colors ? FMT_RESET : "");
}

CLIB_FMT(1, 2) void clib_log_fatal(const char *fmt, ...) {
  _LOG(CLIB_LOG_FATAL, fmt);
}
CLIB_FMT(1, 2) void clib_log_error(const char *fmt, ...) {
  _LOG(CLIB_LOG_ERROR, fmt);
}
CLIB_FMT(1, 2) void clib_log_warning(const char *fmt, ...) {
  _LOG(CLIB_LOG_WARNING, fmt);
}
CLIB_FMT(1, 2) void clib_log_info(const char *fmt, ...) {
  _LOG(CLIB_LOG_INFO, fmt);
}

#ifndef NDEBUG
CLIB_FMT(1, 2) void clib_log_debug(const char *fmt, ...) {
  _LOG(CLIB_LOG_DEBUG, fmt);
}
CLIB_FMT(1, 2) void clib_log_trace(const char *fmt, ...) {
  _LOG(CLIB_LOG_TRACE, fmt);
}
#endif
