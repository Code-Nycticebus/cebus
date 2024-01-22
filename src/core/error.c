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
  err->msg.len = 0;
  err->msg.data = err->buffer;
  err->msg.len += (usize)snprintf(&err->buffer[err->msg.len],
                                  ERROR_MESSAGE_MAX - err->msg.len,
                                  "  [ERROR] %s:%d:\n", file, line);
  va_list va;
  va_start(va, fmt);
  err->msg.len += (usize)vsnprintf(&err->buffer[err->msg.len],
                                   ERROR_MESSAGE_MAX - err->msg.len, fmt, va);
  va_end(va);
  if (err->raise) {
    _error_panic(err);
  }
}

void _error_panic(Error *err) {
  clib_log_fatal("%s:%d:\n" STR_FMT, err->file, err->line, STR_ARG(err->msg));
  abort();
}

void _error_warn(Error *err) {
  clib_log_warning("%s:%d:\n" STR_FMT, err->file, err->line, STR_ARG(err->msg));
  _error_ignore(err);
}

void _error_ignore(Error *err) {
  err->msg.len = 0;
  err->failure = false;
}

void _error_set_code(Error *err, i32 code) { err->code = code; }

void _error_add_note(Error *err, const char *file, int line, const char *fmt,
                     ...) {
  va_list va;
  va_start(va, fmt);
  err->msg.len += (usize)snprintf(&err->buffer[err->msg.len],
                                  ERROR_MESSAGE_MAX - err->msg.len,
                                  "\n  [NOTE] %s:%d:\n", file, line);
  err->msg.len += (usize)vsnprintf(&err->buffer[err->msg.len],
                                   ERROR_MESSAGE_MAX - err->msg.len, fmt, va);
  va_end(va);
}

////////////////////////////////////////////////////////////////////////////
