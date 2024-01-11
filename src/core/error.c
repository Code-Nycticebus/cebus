#include "error.h"

#include "clib/logging.h"

#include <stdarg.h>
#include <stdio.h>

void error_add_note(Error *err, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  err->message[err->msg_size++] = '\n';
  err->msg_size += (usize)vsnprintf(&err->message[err->msg_size],
                                    ERROR_MESSAGE_MAX - err->msg_size, fmt, va);
  va_end(va);
}

void _error_init(Error *err, const char *file, int line, i32 error,
                 const char *fmt, ...) {
  err->failure = true;
  err->file = err->file ? err->file : file;
  err->line = err->line ? err->line : line;
  err->error = error;
  va_list va;
  va_start(va, fmt);
  err->msg_size = (usize)vsnprintf(err->message, ERROR_MESSAGE_MAX, fmt, va);
  va_end(va);
}

void _error_dump(Error *err) {
  clib_log_error("%s:%d:\n%s", err->file, err->line, err->message);
}
