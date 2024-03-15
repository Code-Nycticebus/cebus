/* DOCUMENTATION
## Functions

- **Environment Variables**:
  - `os_getenv(env, error)`: Retrieves the value of an environment variable.

- **Current Working Directory**:
  - `os_getcwd(arena)`: Returns the current working directory, allocating memory from the specified `Arena`.
  - `os_chdir(path)`: Changes the current working directory to the specified path.

## Usage Example

```c
Error error = ErrNew;
Str cwd = os_getcwd(&arena);
Str home = os_getenv("HOME", &error);
error_context(&error, {
  error_raise();
});

printf("Current working directory: %s\n", cwd);
printf("Home directory: %s\n", home);
```
*/

#ifndef __CLIB_OS_H__
#define __CLIB_OS_H__

#include "clib/core/arena.h"
#include "clib/core/defines.h"
#include "clib/core/error.h"

////////////////////////////////////////////////////////////////////////////

Str os_getenv(const char *env, Error *error);
void os_chdir(Str path);
Str os_getcwd(Arena *arena);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_OS_H__ */
