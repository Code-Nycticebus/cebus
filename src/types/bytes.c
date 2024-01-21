#include "bytes.h"

#include "core/defines.h"
#include "types/integers.h"
#include "types/str.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////

Bytes bytes_from_parts(usize size, const u8 *data) {
  return (Bytes){.size = size, .data = data};
}

///////////////////////////////////////////////////////////////////////////////

Bytes bytes_copy(Bytes bytes, Arena *arena) {
  u8 *buffer = arena_alloc(arena, bytes.size);
  memcpy(buffer, bytes.data, bytes.size);
  return bytes_from_parts(bytes.size, buffer);
}

///////////////////////////////////////////////////////////////////////////////

Bytes bytes_slice(Bytes bytes, usize idx1, usize idx2) {
  if (idx2 <= idx1 || bytes.size <= idx1 || bytes.size < idx2) {
    return bytes_from_parts(0, bytes.data);
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

///////////////////////////////////////////////////////////////////////////////

bool bytes_eq(Bytes b1, Bytes b2) {
  if (b1.size != b2.size) {
    return false;
  }
  return memcmp(b1.data, b2.data, b1.size) == 0;
}

///////////////////////////////////////////////////////////////////////////////

Str bytes_to_hex(Bytes bytes, Arena *arena) {
  char *buf = arena_calloc(arena, bytes.size * 2 + 1);
  usize idx = 0;
  for (usize i = 0; i < bytes.size; i++) {
    idx += (usize)snprintf(&buf[idx], 3, "%0*x", (i != 0) + 1, bytes.data[i]);
  }
  return (Str){.len = idx, .data = buf};
}

Bytes bytes_from_hex(Str s, Arena *arena) {
  if (str_startswith(s, STR("0x"))) {
    s = str_substring(s, 2, s.len);
  }

  u8 *buffer = arena_calloc(arena, (s.len / 2) + 1);
  // to convert strings like "0x101".
  // in the first iteration:
  // take 1 or 2 chars depending if s.len is even or odd
  usize idx = 0;
  for (Str ch = {0}; str_try_take(&s, idx == 0 ? 2 - s.len % 2 : 2, &ch);) {
    for (usize i = 0; i < ch.len; i++) {
      buffer[idx] <<= 4;
      if (isdigit(ch.data[i])) {
        buffer[idx] |= (u8)(ch.data[i] - '0');
      } else if (isxdigit(ch.data[i])) {
        const char d = (char)tolower(ch.data[i]);
        buffer[idx] |= 10 + (u8)(d - 'a'); // NOLINT
      }
    }
    idx++;
  }
  return bytes_from_parts(idx, buffer);
}

///////////////////////////////////////////////////////////////////////////////
