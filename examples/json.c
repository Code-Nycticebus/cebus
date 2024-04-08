#include "clib/clib.h"

#include <stdio.h>

typedef struct {
  u32 age;
  Str name;
} User;

const User users[] = {
    {.name = STR_STATIC("Loris"), .age = 25},
    {.name = STR_STATIC("Michi"), .age = 21},
    {.name = STR_STATIC("José"), .age = 22},
};
const usize user_count = ARRAY_LEN(users);

static void user_to_json(const User *self, StringBuilder *sb) {
  sb_append_str(sb, STR("{"));
  {
    sb_append_fmt(sb, "\"name\": \"" STR_FMT "\",", STR_ARG(self->name));
    sb_append_fmt(sb, "\"age\": %u", self->age);
  }
  sb_append_str(sb, STR("}"));
}

int main(void) {
  Arena arena = {0};
  StringBuilder sb = sb_init(&arena);

  sb_append_str(&sb, STR("{\"users\": ["));
  for (usize i = 0; i < user_count; ++i) {
    if (i != 0) {
      sb_append_str(&sb, STR(", "));
    }
    user_to_json(&users[i], &sb);
  }
  sb_append_str(&sb, STR("]}"));

  Str content = sb_to_str(&sb);
  printf(STR_FMT "\n", STR_ARG(content));

  arena_free(&arena);
}
