#include "os/cmd.h"

#include <stdio.h>

int main(void) {
  printf("HELLO\n");
  Error err = ErrNew;
  cmd_exec(&err, 2, (Str[]){STR("ls"), STR("src")});
  error_context(&err, { error_panic(); });
  printf("LETS GO\n");
}
