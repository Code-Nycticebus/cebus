#include "arena.h"
#include <stdio.h>
#include <string.h>
int main(void) {
  Arena *arena = arena_make();

  char *buffer = arena_alloc(arena, 900);
  strcpy(buffer, "Hello1");
  printf("%s\n", buffer);

  char *buffer2 = arena_alloc(arena, 300);
  strcpy(buffer2, "Hello2");
  printf("%s\n", buffer2);

  char *buffer3 = arena_calloc(arena, 1000);
  buffer3[0] = 'a';
  buffer3[1] = 'b';
  buffer3[2] = 'c';
  printf("%s\n", buffer3);

  arena_free(arena);
}
