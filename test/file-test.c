#include "clib/arena.h"
#include "clib/asserts.h"
#include "clib/logging.h"
#include "datatypes/bytes.h"
#include "datatypes/datatypes.h"
#include "datatypes/str.h"
#include "datatypes/utf8.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define LINE_MAX 512

typedef struct {
  FILE *file;
  Arena arena;
} FileBuffer;

bool file_try_open(const char *path, const char *mode, FILE **file) {
  Arena arena = {0};
  *file = fopen(path, mode);
  clib_assert_return(*file, false);
  arena_free(&arena);
  return true;
}

FILE *file_open(const char *path, const char *mode) {
  FILE *file = NULL;
  clib_assert(file_try_open(path, mode, &file), "Could not open file: '%s': %s",
              path, strerror(errno));
  return file;
}

usize file_size(FILE *file) {
  clib_assert_debug(file, "could not get size. file handle invalid");
  const usize current = ftell(file);
  fseek(file, 0, SEEK_END);
  const usize size = ftell(file);
  fseek(file, current, SEEK_SET);
  return size;
}

Bytes file_load_bytes(FILE *file, Arena *arena) {
  const usize size = file_size(file);
  u8 *buffer = arena_alloc(arena, size);
  fread(buffer, sizeof(buffer[0]), size, file);
  return bytes_from_parts(size, buffer);
}

Str file_load_str(FILE *file, Arena *arena) {
  const usize size = file_size(file);
  char *buffer = arena_alloc(arena, size + 1);
  buffer[size] = '\0';
  fread(buffer, sizeof(buffer[0]), size, file);
  return str_from_parts(size, buffer);
}

Utf8 file_load_utf8(FILE *file, Arena *arena) {
  const usize size = file_size(file);
  u8 *buffer = arena_alloc(arena, size);
  fread(buffer, sizeof(buffer[0]), size, file);
  return utf8_decode(bytes_from_parts(size, buffer));
}

void file_close(FILE *file) { fclose(file); }

int main(void) {
  Arena arena = {0};

  {
    FILE *file = file_open("test/res/text.txt", "w");
    Str b = STR("[config]\n"
                "test=32\n"
                "test2 = \"Hello fucker\"\n");
    fwrite(b.data, sizeof(b.data[0]), b.len, file);
    file_close(file);
    arena_reset(&arena);
  }

  {
    FILE *file = file_open("test/res/text.txt", "r");
    Str content = file_load_str(file, &arena);
    for (Str line = {0}; str_try_chop_by_delim(&content, '\n', &line);) {
      clib_log_info(STR_FMT, STR_ARG(line));
    }
    file_close(file);
    arena_reset(&arena);
  }

  {
    FILE *file = file_open("test/res/utf.txt", "wb");
    Utf8 b = UTF8("🎉✅😁cA😁");
    fwrite(b.data, sizeof(b.data[0]), b.size, file);
    file_close(file);
    arena_reset(&arena);
  }

  {
    FILE *file = file_open("test/res/utf.txt", "r");
    Utf8 u = file_load_utf8(file, &arena);
    for (Utf8 s = u, c = {0}; utf8_try_next(&s, &c);) {
      clib_log_info(UTF8_FMT, UTF8_ARG(c));
    }
    file_close(file);
    arena_reset(&arena);
  }

  {
    FILE *file = file_open("test/res/bytes.txt", "w");
    u8 buffer[U8_MAX + 1];
    for (usize i = 0; i < U8_MAX + 1; ++i) {
      buffer[i] = (u8)i;
    }

    Bytes b = bytes_from_parts(U8_MAX + 1, buffer);
    fwrite(b.data, sizeof(u8), b.size, file);

    file_close(file);
    arena_reset(&arena);
  }

  {
    FILE *file = file_open("test/res/text.txt", "rb");
    Bytes u = file_load_bytes(file, &arena);

    Str s = bytes_hex(u, &arena);
    clib_log_info("\n" STR_FMT, STR_ARG(s));

    file_close(file);
    arena_reset(&arena);
  }

  {
    FILE *file = file_open("test/res/utf.txt", "rb");
    Bytes u = file_load_bytes(file, &arena);

    Str s = bytes_hex(u, &arena);
    clib_log_info("\n" STR_FMT, STR_ARG(s));

    file_close(file);
    arena_reset(&arena);
  }

  {
    FILE *file = file_open("test/res/bytes.txt", "rb");
    Bytes u = file_load_bytes(file, &arena);

    Str s = bytes_hex(u, &arena);
    clib_log_info("\n" STR_FMT, STR_ARG(s));

    file_close(file);
    arena_reset(&arena);
  }

  arena_free(&arena);
}
