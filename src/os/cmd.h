#ifndef __CLIB_CMD_H__
#define __CLIB_CMD_H__

#include "core/defines.h"
#include "core/error.h"

typedef enum {
  CMD_OK,
  CMD_FORK,
  CMD_NOT_FOUND = 127,
} CmdError;

void cmd_exec(Error *error, size_t argc, Str *argv);

#endif // !__CLIB_CMD_H__
