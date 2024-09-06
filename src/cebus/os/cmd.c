#include "cmd.h"

#include "cebus/collection/da.h"
#include "cebus/core/arena.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cmd_exec_da(Error *error, const Cmd *cmd) { cmd_exec(error, cmd->len, cmd->items); }

////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)

#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

void cmd_exec(Error *error, size_t argc, Str *argv) {
  errno = 0;
  pid_t pid = fork();
  if (pid == -1) {
    error_emit(error, CMD_FORK, "fork failed: %s", strerror(errno));
  } else if (pid == 0) {
    Arena arena = {0};

    DA(char *) args = {0};
    da_init(&args, &arena);
    for (size_t i = 0; i < argc; i++) {
      char *cmd = arena_calloc(&arena, argv[i].len + 1);
      strncpy(cmd, argv[i].data, argv[i].len);
      da_push(&args, cmd);
    }
    da_push(&args, NULL);
    execvp(args.items[0], args.items);

    arena_free(&arena);
    exit(CMD_NOT_FOUND);
  }

  int status = 0;
  waitpid(pid, &status, 0);
  if (WIFEXITED(status)) {
    int exit_code = WEXITSTATUS(status);
    if (exit_code == CMD_NOT_FOUND) {
      error_emit(error, CMD_NOT_FOUND, "command not found: " STR_FMT, STR_ARG(argv[0]));
    } else if (exit_code != 0) {
      error_emit(error, exit_code, "command failed: " STR_FMT ": exit code: %d\n", STR_ARG(argv[0]),
                 exit_code);
    }
  }
}

////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include "cebus/type/string.h"

void cmd_exec(Error *error, size_t argc, Str *argv) {
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  Arena arena = {0};

  Str command = str_wrap(argv[0], STR("\""), &arena);
  Str args = str_join(STR(" "), argc - 1, argv + 1, &arena);
  char *cmd = arena_calloc(&arena, command.len + 1 + args.len + 1);
  strncpy(cmd, command.data, command.len);
  cmd[command.len] = ' ';
  strncpy(cmd + command.len + 1, args.data, args.len);

  if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
    DWORD ec = GetLastError();
    if (ec == 0x2) {
      error_emit(error, CMD_NOT_FOUND, "command not found: " STR_FMT, STR_ARG(argv[0]));
    } else {
      error_emit(error, (i32)ec, "command creation failed: " STR_FMT ": %lu", STR_ARG(argv[0]), ec);
    }
    goto defer;
  }
  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exit_code = 0;
  if (!GetExitCodeProcess(pi.hProcess, &exit_code)) {
    DWORD ec = GetLastError();
    error_emit(error, (i32)ec, "Could not get exit code of process: " STR_FMT ": %lu",
               STR_ARG(argv[0]), ec);
    goto defer;
  }
  if (exit_code != 0) {
    error_emit(error, (i32)exit_code, "command failed: " STR_FMT ": %lu", STR_ARG(command),
               exit_code);
    goto defer;
  }

defer:
  arena_free(&arena);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

////////////////////////////////////////////////////////////////////////////
#endif
