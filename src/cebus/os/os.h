/* DOCUMENTATION
## Functions

- **Environment Variables**:
  - `os_getenv(env, error)`: Retrieves the value of an environment variable.

- **Current Working Directory**:
  - `os_getcwd(arena)`: Returns the current working directory, allocating memory
from the specified `Arena`.
  - `os_chdir(path)`: Changes the current working directory to the specified
path.

## Usage Example

```c
Error error = ErrNew;
Str cwd = os_getcwd(&arena);
Str home = os_getenv("HOME", &error);
error_context(&error, {
  error_raise();
});

printf("Current working directory: " STR_FMT "\n", STR_ARG(cwd));
printf("Home directory: " STR_FMT "\n", STR_ARG(home));
```
*/

#ifndef __CEBUS_OS_H__
#define __CEBUS_OS_H__

#include "cebus/core/arena.h"
#include "cebus/core/defines.h"
#include "cebus/core/error.h"

////////////////////////////////////////////////////////////////////////////

Str os_getenv(const char *env, Error *error);
void os_mkdir(Path path);
void os_mkdir_mode(Path path, u32 mode);
void os_chdir(Path path);
Path os_getcwd(Arena *arena);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_OS_H__ */
