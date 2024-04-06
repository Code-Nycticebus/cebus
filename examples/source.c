/* Converts a files bytes into a byte array and saves it as a .h file */

#include "clib/clib.h"

int main(int argc, const char **argv) {
  if (argc < 3) {
    clib_log_error("usage: %s <input> <output>.h", argv[0]);
    return -1;
  }

  Str input_path = str_from_cstr(argv[1]);
  Str output_path = str_from_cstr(argv[2]);

  Arena arena = {0};

  Error error = ErrNew;
  Bytes content = file_read_bytes(input_path, &arena, &error);
  error_context(&error, {
    if (error_code(i32) == FILE_NOT_FOUND) {
      clib_log_error(STR_FMT, STR_ARG(error_msg()));
      return -2;
    }
  });

  if (!str_endswith(output_path, STR(".h"))) {
    clib_log_error("Output path incorrect: " STR_FMT, STR_ARG(output_path));
    return -1;
  }

  // Open file
  FILE *out = file_open(output_path, "w", &error);
  error_context(&error, {
    if (error_code(i32) == FILE_NOT_FOUND) {
      clib_log_error(STR_FMT, STR_ARG(error_msg()));
      return -2;
    }
  });

  // Extract name from path
  Str name = str_chop_right_by_delim(&output_path, '/');
  name = str_chop_by_delim(&name, '.');
  Str upper_name = str_upper(name, &arena);

  // Write Header guard
  io_write_fmt(out, "#ifndef __" STR_FMT "_H__\n", STR_ARG(upper_name));
  io_write_fmt(out, "#define __" STR_FMT "_H__\n\n", STR_ARG(upper_name));

  // Write size
  io_write_fmt(out,
               "const unsigned long long " STR_FMT "_size = %" USIZE_FMT ";\n",
               STR_ARG(name), content.size);

  // Write content
  io_write_fmt(out, "const unsigned char " STR_FMT "[] = {", STR_ARG(name));
  for (usize i = 0; i < content.size; ++i) {
    if (i % 12 == 0) {
      io_write_fmt(out, "\n\t");
    }
    usize s = io_write_fmt(out, "0x%x,", content.data[i]);
    if (i % 12 != 11) {
      io_write_fmt(out, "%*s", 6 - (int)s, "");
    }
  }
  io_write_fmt(out, "\n};\n\n");

  // End header guard
  io_write_fmt(out, "#endif /* !__" STR_FMT "_H__ */\n", STR_ARG(upper_name));

  // cleanup
  file_close(out, &error);
  error_context(&error, { error_panic(); });
  arena_free(&arena);
}
