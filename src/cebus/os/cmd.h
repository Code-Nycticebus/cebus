/* DOCUMENTATION
## Functions

- **`cmd_exec(error, argc, argv)`**: Executes a system command.

## Error Handling

Defines `CmdError` enum for different command execution outcomes:
- `CMD_OK`: Command executed successfully.
- `CMD_FORK`: Error occurred while forking the process.
- `CMD_NOT_FOUND`: Command not found, typically returns `127`.

## Usage Example

```c
Error error = ErrNew;
Str args[] = {STR("/bin/echo"), STR("Hello, world!")};
cmd_exec(&error, 2, args);
error_context(&error, {
    // Handle error
});
```
*/

#ifndef __CEBUS_CMD_H__
#define __CEBUS_CMD_H__

#include "cebus/core/defines.h"
#include "cebus/core/error.h"

typedef enum {
  CMD_OK,
  CMD_FORK,
  CMD_NOT_FOUND = 127,
} CmdError;

void cmd_exec(Error *error, size_t argc, Str *argv);

#endif // !__CEBUS_CMD_H__
