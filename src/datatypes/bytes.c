#include "bytes.h"
#include "core/defines.h"
#include "datatypes/integers.h"
#include "datatypes/str.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

Bytes bytes_from_parts(usize size, const u8 *data) {
  return (Bytes){.size = size, .data = data};
}

Bytes bytes_copy(Bytes bytes, Arena *arena) {
  u8 *buffer = arena_alloc(arena, bytes.size);
  memcpy(buffer, bytes.data, bytes.size);
  return bytes_from_parts(bytes.size, buffer);
}

bool bytes_eq(Bytes b1, Bytes b2) {
  if (b1.size != b2.size) {
    return false;
  }
  return memcmp(b1.data, b2.data, b1.size) == 0;
}

Bytes bytes_slice(Bytes bytes, usize idx1, usize idx2) {
  if (idx2 <= idx1 || bytes.size <= idx1 || bytes.size < idx2) {
    return bytes_from_parts(0, &bytes.data[bytes.size - 1]);
  }
  return bytes_from_parts(idx2 - idx1, &bytes.data[idx1]);
}

Bytes bytes_take(Bytes *bytes, usize count) {
  count = usize_min(bytes->size, count);
  Bytes ret = bytes_from_parts(count, bytes->data);
  bytes->size -= count;
  bytes->data += count;
  return ret;
}

Str bytes_hex(Bytes bytes, Arena *arena) {
  char *buffer = arena_calloc(arena, bytes.size * 2 + 1);
  usize b_idx = 0;
  for (usize i = 0; i < bytes.size; i++) {
    b_idx += (usize)snprintf(&buffer[b_idx], 3, "%02x", bytes.data[i]);
  }
  return (Str){.len = b_idx, .data = buffer};
}

Bytes bytes_from_hex(Str s, Arena *arena) {
  if (str_startswith(s, STR("0x"))) {
    s = str_substring(s, 2, s.len);
  }

  u8 *buffer = arena_calloc(arena, s.len);
  usize idx = 0;
  for (Str chunk = {0}; str_try_take(&s, 2, &chunk);) {
    for (usize i = 0; i < chunk.len; i++) {
      buffer[idx] <<= 4;
      if (isdigit(chunk.data[i])) {
        buffer[idx] |= (u8)(chunk.data[i] - '0');
      } else if (isxdigit(chunk.data[i])) {
        const char d = (char)tolower(chunk.data[i]);
        buffer[idx] = 10 + (u8)(d - 'a');
      }
    }
    idx++;
  }
  return bytes_from_parts(idx, buffer);
}
