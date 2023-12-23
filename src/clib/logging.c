#include "logging.h"

#include "defines.h" // IWYU pragma: export
#include <stdarg.h>
#include <stdio.h>

#include "platform.h"

#define CLIB_LOGGER_CHAR_BUFFER_SIZE 1000

#if defined(LINUX)
#include <unistd.h>
#elif defined(WINDOWS)
#include <io.h>
#define isatty(...) _isatty(__VA_ARGS__)
#else
#define isatty(...) false
#endif

struct CmLogLevelPrefix {
  const char *prefix;
  const char *color;
};

#define FMT_RESET "\033[0m"

#define _LOG(__log_level, __file, __fmt)                                       \
  char __buffer[CLIB_LOGGER_CHAR_BUFFER_SIZE];                                 \
  va_list __args;                                                              \
  va_start(__args, __fmt);                                                     \
  vsnprintf(__buffer, CLIB_LOGGER_CHAR_BUFFER_SIZE, __fmt, __args);            \
  va_end(__args);                                                              \
  _clib_log(__log_level, __file, __buffer);

static void _clib_log(LogLevel log_level, FILE *file, const char *msg) {
  static bool display_colors = false;
  static bool tty_checked = false;
  if (!tty_checked) {
    tty_checked = true;
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

  fprintf(file, "%s[%s]: %s%s\n",
          display_colors ? log_level_str[log_level].color : "",
          log_level_str[log_level].prefix, msg,
          display_colors ? FMT_RESET : "");
}

CLIB_FMT(3, 4)
void clib_log(FILE *f, LogLevel log_level, const char *fmt, ...) {
  _LOG(log_level, f, fmt);
}

CLIB_FMT(1, 2) void clib_log_fatal(const char *fmt, ...) {
  _LOG(CLIB_LOG_FATAL, stderr, fmt);
}
CLIB_FMT(1, 2) void clib_log_error(const char *fmt, ...) {
  _LOG(CLIB_LOG_ERROR, stderr, fmt);
}
CLIB_FMT(1, 2) void clib_log_warning(const char *fmt, ...) {
  _LOG(CLIB_LOG_WARNING, stderr, fmt);
}
CLIB_FMT(1, 2) void clib_log_info(const char *fmt, ...) {
  _LOG(CLIB_LOG_INFO, stdout, fmt);
}

#ifndef NDEBUG
CLIB_FMT(1, 2) void clib_log_debug(const char *fmt, ...) {
  _LOG(CLIB_LOG_DEBUG, stdout, fmt);
}
CLIB_FMT(1, 2) void clib_log_trace(const char *fmt, ...) {
  _LOG(CLIB_LOG_TRACE, stdout, fmt);
}
#endif
