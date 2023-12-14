#include "arena.h"
#include "da.h"
#include "str.h"

#include <stdio.h>
#include <string.h>

typedef struct {
  DA(int);
} ListI32;

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

  ListI32 list = {0};
  da_init(&list, 2);
  for (size_t i = 0; i < 10; ++i) {
    da_push(&list, i + 1);
  }

  printf("{");
  for (size_t i = 0; i < list.len; ++i) {
    if (i != 0) {
      printf(", ");
    }
    printf("%d", list.items[i]);
  }
  printf("}\n");

  da_free(&list);

  Str str = STR("Hello");
  Str str2 = str_copy(arena, str);
  Str str3 = str_from_cstr(", World");
  Str full = str_cat(arena, str2, str3);

  printf(STR_FMT "\n", STR_ARG(full));

  arena_free(arena);
}
