#include "logging.h"

#include <stdarg.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////

#if defined(LINUX)
#include <unistd.h>
#elif defined(WINDOWS)
#include <io.h>
#define isatty(...) _isatty(__VA_ARGS__)
#else
#define isatty(...) false
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 1
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 2
#endif

////////////////////////////////////////////////////////////////////////////

struct CmLogLevelPrefix {
  const char *prefix;
  const char *color;
  int file;
};

static const struct CmLogLevelPrefix log_level_str[] = {
    [CEBUS_LOG_FATAL] = {"FATAL", "\033[1m\033[91m", STDERR_FILENO},
    [CEBUS_LOG_ERROR] = {"ERROR", "\033[91m", STDERR_FILENO},
    [CEBUS_LOG_WARNING] = {"WARNING", "\033[33m", STDERR_FILENO},
    [CEBUS_LOG_INFO] = {"INFO", "\033[34m", STDOUT_FILENO},
    [CEBUS_LOG_DEBUG] = {"DEBUG", "\033[1m\033[95m", STDOUT_FILENO},
    [CEBUS_LOG_TRACE] = {"TRACE", "\033[97m", STDOUT_FILENO},
};

#define FMT_RESET "\033[0m"

////////////////////////////////////////////////////////////////////////////

#define _LOG(__log_level, __fmt)                                               \
  _cebus_log(__log_level);                                                      \
  va_list __args;                                                              \
  va_start(__args, __fmt);                                                     \
  vfprintf(log_level_str[__log_level].file == STDERR_FILENO ? stderr : stdout, \
           __fmt, __args);                                                     \
  va_end(__args);                                                              \
  fprintf(log_level_str[__log_level].file == STDERR_FILENO ? stderr : stdout,  \
          "%s\n", display_colors ? FMT_RESET : "");

static bool display_colors = false;
static bool tty_checked = false;

static void _cebus_log(LogLevel log_level) {
  if (UNLIKELY(!tty_checked)) {
    tty_checked = true;
    display_colors = isatty(STDOUT_FILENO) && isatty(STDERR_FILENO);
  }

  fprintf(log_level_str[log_level].file == STDERR_FILENO ? stderr : stdout,
          "%s[%s]: ", display_colors ? log_level_str[log_level].color : "",
          log_level_str[log_level].prefix);
}

////////////////////////////////////////////////////////////////////////////

void cebus_log_level(LogLevel level, const char *fmt, ...) { _LOG(level, fmt); }

void cebus_log(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  printf("\n");
  va_end(args);
}
void cebus_log_fatal(const char *fmt, ...) { _LOG(CEBUS_LOG_FATAL, fmt); }
void cebus_log_error(const char *fmt, ...) { _LOG(CEBUS_LOG_ERROR, fmt); }
void cebus_log_warning(const char *fmt, ...) { _LOG(CEBUS_LOG_WARNING, fmt); }
void cebus_log_info(const char *fmt, ...) { _LOG(CEBUS_LOG_INFO, fmt); }

////////////////////////////////////////////////////////////////////////////

void _cebus_log_debug(const char *fmt, ...) { _LOG(CEBUS_LOG_DEBUG, fmt); }
void _cebus_log_trace(const char *fmt, ...) { _LOG(CEBUS_LOG_TRACE, fmt); }

////////////////////////////////////////////////////////////////////////////
