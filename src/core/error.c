#include "error.h"

#include "core/asserts.h"
#include "core/logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////

bool error_occured(Error *err) { return err && err->failure; }

void error_raise(Error *err) {
  clib_assert_debug(err, "Error is 'NULL'!");
  clib_log_fatal("%s:%d:\n%s", err->file, err->line, err->message);
  abort();
}

void error_warn(Error *err) {
  clib_assert_debug(err, "Error is 'NULL'!");
  clib_log_warning("%s:%d:\n%s", err->file, err->line, err->message);
}

////////////////////////////////////////////////////////////////////////////

void _error_init(Error *err, i32 code, const char *file, int line,
                 const char *fmt, ...) {
  err->failure = true;
  err->code = code;
  err->msg_size = 0;
  err->msg_size += (usize)snprintf(&err->message[err->msg_size],
                                   ERROR_MESSAGE_MAX - err->msg_size,
                                   "  [ERROR] %s:%d:\n", file, line);
  va_list va;
  va_start(va, fmt);
  err->msg_size += (usize)vsnprintf(&err->message[err->msg_size],
                                    ERROR_MESSAGE_MAX - err->msg_size, fmt, va);
  va_end(va);
  if (err->raise) {
    error_raise(err);
  }
}

void _error_add_note(Error *err, const char *file, int line, const char *fmt,
                     ...) {
  clib_assert_debug(err, "%s:%d: Error is 'NULL'!", file, line);
  va_list va;
  va_start(va, fmt);
  err->msg_size += (usize)snprintf(&err->message[err->msg_size],
                                   ERROR_MESSAGE_MAX - err->msg_size,
                                   "\n  [NOTE] %s:%d:\n", file, line);
  err->msg_size += (usize)vsnprintf(&err->message[err->msg_size],
                                    ERROR_MESSAGE_MAX - err->msg_size, fmt, va);
  va_end(va);
}

////////////////////////////////////////////////////////////////////////////
