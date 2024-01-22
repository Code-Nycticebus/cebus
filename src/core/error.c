#include "error.h"

#include "core/asserts.h"
#include "core/logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////

void _error_emit(Error *err, i32 code, const char *file, int line,
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
    _error_panic(err);
  }
}

void _error_panic(Error *err) {
  clib_log_fatal("%s:%d:\n%s", err->file, err->line, err->message);
  abort();
}

void _error_warn(Error *err) {
  clib_log_warning("%s:%d:\n%s", err->file, err->line, err->message);
  _error_ignore(err);
}

void _error_ignore(Error *err) {
  err->msg_size = 0;
  err->failure = false;
}

void _error_set_code(Error *err, i32 code) { err->code = code; }

void _error_add_note(Error *err, const char *file, int line, const char *fmt,
                     ...) {
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
