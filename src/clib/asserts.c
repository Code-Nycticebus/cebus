#include "asserts.h"

#include "logging.h"

#include <stdlib.h>

void _clib_assert_print_error(const char *msg, const char *expression,
                              const char *filename, int line) {
  clib_log_error("%s:%d: assert '%s' failed: %s", filename, line, expression,
                 msg);
}

void _clib_assert_print_warning(const char *msg, const char *expression,
                                const char *filename, int line) {
  clib_log_warning("%s:%d: assert '%s' failed: %s", filename, line, expression,
                   msg);
}
