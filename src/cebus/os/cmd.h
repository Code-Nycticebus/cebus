/* DOCUMENTATION
## Functions

- **`cmd_exec(error, argc, argv)`**: Executes a system command.
- **`cmd_exec_da(error, da)`**: Executes a with a dynamic array.

## Construction a dynamic array

```c
Arena arena = {0};

Cmd cmd = cmd_new(&arena);

cmd_push(&cmd, STR("gcc"), STR("-o"), STR("main"));

Str cflags[] = {STR("-Wall"), STR("-Wextra")};
cmd_extend(&cmd, words);

DA(Str) files = da_new(&arena);
// collect files...
cmd_extend_da(&cmd, &files);

cmd_exec_da(ErrPanic, &cmd);

arena_free(&arena);
```

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

#include "cebus/collection/da.h"
#include "cebus/core/defines.h"
#include "cebus/core/error.h"

typedef enum {
  CMD_OK,
  CMD_FORK,
  CMD_NOT_FOUND = 127,
} CmdError;

void cmd_exec(Error *error, size_t argc, Str *argv);

typedef DA(Str) Cmd;

#define cmd_new(arena) da_new(arena)
#define cmd_push(cmd, ...) da_extend(cmd, ARRAY_LEN((Str[]){__VA_ARGS__}), (Str[]){__VA_ARGS__})
#define cmd_extend(cmd, ...) da_extend(cmd, ARRAY_LEN(__VA_ARGS__), (__VA_ARGS__))
#define cmd_extend_da(cmd, da) da_extend(cmd, (da)->len, (da)->items)

void cmd_exec_da(Error *error, const Cmd *cmd);

#endif // !__CEBUS_CMD_H__
