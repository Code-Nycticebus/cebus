#include "os.h"

#include "types/str.h"

#include <stdlib.h>

Str os_getenv(const char *env, Error *error) {
  const char *var = getenv(env);
  if (var == NULL) {
    Err(error, 0, "Could not get environment variable: %s", env);
    return (Str){0};
  }
  return str_from_cstr(var);
}
