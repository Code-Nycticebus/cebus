#include "clib/clib.h"

#include <stdio.h>

typedef struct {
  u32 age;
  Str name;
} User;

static void user_to_html(const User *self, StringBuilder *sb) {
  sb_append_str(sb, STR("<span>"));
  sb_append_fmt(sb, "<p>Name: " STR_FMT "</p>", STR_ARG(self->name));
  sb_append_fmt(sb, "<p>Age: %u</p>", self->age);
  sb_append_str(sb, STR("</span>"));
}

int main(void) {
  const User users[] = {
      {.name = STR("Loris"), .age = 25},
      {.name = STR("Michi"), .age = 21},
      {.name = STR("José"), .age = 22},
  };
  const usize user_count = ARRAY_LEN(users);

  Arena arena = {0};
  StringBuilder sb = sb_init(&arena);

  sb_append_str(&sb, STR("<ul>\n"));
  for (usize i = 0; i < user_count; i++) {
    sb_append_str(&sb, STR("\t<li>"));
    user_to_html(&users[i], &sb);
    sb_append_str(&sb, STR("</li>\n"));
  }
  sb_append_str(&sb, STR("</ul>"));

  Str content = sb_to_str(&sb);
  printf(STR_FMT "\n", STR_ARG(content));

  arena_free(&arena);
}
