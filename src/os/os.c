#include "os.h"

#include "type/string.h"

#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////

Str os_getenv(const char *env, Error *error) {
  const char *var = getenv(env);
  if (var == NULL) {
    error_emit(error, 0, "Could not get environment variable: %s", env);
    return (Str){0};
  }
  return str_from_cstr(var);
}

////////////////////////////////////////////////////////////////////////////
