#include "error.h"

#include "clib/type/string.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

static void error_dump(ErrorInfo *info) {
  fprintf(stderr, "[ERROR] %s:%d\n", info->file, info->line);
  if (info->location_count > ERROR_LOCATIONS_MAX) {
    fprintf(stderr, "  [%" USIZE_FMT "] ...\n",
            info->location_count - ERROR_LOCATIONS_MAX);
  }
  for (usize i = 0; i < info->location_idx; ++i) {
    fprintf(stderr, "  [%" USIZE_FMT "] %s:%d\n",
            info->location_count < ERROR_LOCATIONS_MAX
                ? i + 1
                : (info->location_count - ERROR_LOCATIONS_MAX) + i + 1,
            info->locations[i].file, info->locations[i].line);
  }

  fprintf(stderr, "\n");
  Str notes = str_from_parts(info->msg_len, info->msg_buffer);
  for (Str note = {0}; str_try_chop_by_delim(&notes, '\0', &note);) {
    fprintf(stderr, STR_FMT "\n", STR_ARG(note));
  }
}

static void error_location_push(ErrorInfo *info, const char *file, int line) {
  if (info->location_idx > 0) {
    if (info->locations[info->location_idx - 1].line == line &&
        strcmp(info->locations[info->location_idx - 1].file, file) == 0) {
      return;
    }
  }
  if (!(info->location_idx < ERROR_LOCATIONS_MAX)) {
    for (usize i = 0; i < info->location_idx - 1; ++i) {
      info->locations[i] = info->locations[i + 1];
    }
    info->location_idx--;
  }
  info->locations[info->location_idx].file = file;
  info->locations[info->location_idx].line = line;
  info->location_idx++;
  info->location_count++;
}

////////////////////////////////////////////////////////////////////////////

void _error_emit(Error *err, i32 code, const char *file, int line,
                 const char *fmt, ...) {
  err->failure = true;
  err->info.code = code;

  error_location_push(&err->info, file, line);

  va_list va;
  va_start(va, fmt);
  err->info.msg_len = 0;
  err->info.msg_len +=
      (usize)vsnprintf(&err->info.msg_buffer[err->info.msg_len],
                       ERROR_BUFFER_MAX - err->info.msg_len, fmt, va);
  va_end(va);

  err->info.msg = str_from_parts(err->info.msg_len, err->info.msg_buffer);
  err->info.msg_len++;

  if (err->panic_on_emit) {
    _error_panic(err);
  }
}

bool _error_occured(Error *err) { return err && err->failure; }

void _error_panic(Error *err) {
  error_dump(&err->info);
  abort();
}

void _error_except(Error *err) {
  err->info.msg_len = 0;
  err->info.location_idx = 0;
  err->info.location_count = 0;
  err->info.msg = (Str){0};
  err->failure = false;
}

void _error_set_code(Error *err, i32 code) { err->info.code = code; }

void _error_set_msg(Error *err, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  usize size = (usize)vsnprintf(&err->info.msg_buffer[err->info.msg_len],
                                ERROR_BUFFER_MAX - err->info.msg_len, fmt, va);
  err->info.msg =
      str_from_parts(size, &err->info.msg_buffer[err->info.msg_len]);
  err->info.msg_len += size + 1;

  va_end(va);
}

void _error_add_note(Error *err, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  err->info.msg_len +=
      (usize)vsnprintf(&err->info.msg_buffer[err->info.msg_len],
                       ERROR_BUFFER_MAX - err->info.msg_len, fmt, va);
  err->info.msg_len++;

  va_end(va);
}

void _error_add_location(Error *err, const char *file, int line) {
  error_location_push(&err->info, file, line);
}

////////////////////////////////////////////////////////////////////////////
