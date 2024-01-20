#include "error.h"

#include "core/logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////

void error_add_note(Error *err, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  err->message[err->msg_size++] = '\n';
  err->msg_size += (usize)vsnprintf(&err->message[err->msg_size],
                                    ERROR_MESSAGE_MAX - err->msg_size, fmt, va);
  va_end(va);
}

void error_raise(Error *err) {
  clib_log_fatal("%s:%d:\n%s", err->file, err->line, err->message);
  abort();
}

void error_warn(Error *err) {
  clib_log_warning("%s:%d:\n%s", err->file, err->line, err->message);
}

////////////////////////////////////////////////////////////////////////////

void _error_init(Error *err, i32 error, const char *fmt, ...) {
  err->failure = true;
  err->error = error;
  va_list va;
  va_start(va, fmt);
  err->msg_size = (usize)vsnprintf(err->message, ERROR_MESSAGE_MAX, fmt, va);
  va_end(va);
  if (err->raise) {
    error_raise(err);
  }
}

////////////////////////////////////////////////////////////////////////////
