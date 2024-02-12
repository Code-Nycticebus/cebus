#include "cmd.h"

#include "collection/da.h"
#include "core/arena.h"
#include "core/platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
      error_emit(error, CMD_NOT_FOUND, "command not found: " STR_FMT,
                 STR_ARG(argv[0]));
    } else if (exit_code != 0) {
      error_emit(error, exit_code,
                 "command failed: " STR_FMT ": exit code: %d\n",
                 STR_ARG(argv[0]), exit_code);
    }
  }
}

////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include "type/string.h"
#include <windows.h>

void cmd_exec(Error *error, size_t argc, Str *argv) {
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  Arena arena = {0};

  Str cmd = str_join_wrap(STR(" "), STR("\""), argc, argv, &arena);

  if (!CreateProcessA(NULL, cmd.data, NULL, NULL, false, 0, NULL, NULL, &si,
                      &pi)) {
    error_emit(error, -1, "command failed: " STR_FMT ": %d", STR_ARG(argv[0]),
               GetLastError());
  }
  WaitForSingleObject(pi.hProcess, INFINITE);

  arena_free(&arena);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

////////////////////////////////////////////////////////////////////////////
#endif
