#include "bytes.h"
#include "datatypes/datatypes.h"
#include "datatypes/integers.h"
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
    return BYTES_STR("");
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

static usize _log(usize s) {
  usize count = 1;
  while ((s /= 10)) {
    count++;
  }
  return count;
}

Str bytes_hex(Bytes bytes, Arena *arena) {
  const usize padded_size = bytes.size + (8 - (bytes.size % 8));
  const usize lines = usize_max(padded_size / 8, 1);
  const usize bytes_count = 8;
  const usize number_size = _log(bytes.size) + 1;
  const usize starting_size = 4 + number_size;
  const usize between_size = 3;
  const usize bytes_per_line = starting_size + (bytes_count * 2) +
                               (bytes_count - 1) + between_size + (bytes_count);
  const usize new_size = lines * bytes_per_line;

  char *buffer = arena_calloc(arena, new_size + 1);
  usize b_idx = 0;
  for (usize i = 0; (i * bytes_count) < bytes.size; i++) {
    if (i != 0) {
      buffer[b_idx++] = '\n';
    }

    b_idx += snprintf(&buffer[b_idx], starting_size + 1, "|%*." USIZE_FMT " | ",
                      (int)number_size,
                      (i + 1) * bytes_count < bytes.size ? (i + 1) * bytes_count
                                                         : bytes.size);

    for (usize j = 0; j < bytes_count; j++) {
      usize idx = (i * bytes_count) + j;
      if (j != 0) {
        buffer[b_idx++] = ' ';
      }
      if (idx < bytes.size) {
        b_idx += snprintf(&buffer[b_idx], 3, "%02x", bytes.data[(i * 8) + j]);
      } else {
        buffer[b_idx++] = ' ';
        buffer[b_idx++] = ' ';
      }
    }

    b_idx += snprintf(&buffer[b_idx], between_size + 1, " | ");

    for (size_t j = 0; j < bytes_count; j++) {
      usize idx = (i * bytes_count) + j;
      if (idx < bytes.size) {
        if (isprint(bytes.data[idx])) {
          buffer[b_idx++] = bytes.data[idx];
        } else {
          buffer[b_idx++] = '.';
        }
      } else {
        buffer[b_idx++] = ' ';
      }
    }
  }
  return (Str){.len = b_idx, .data = buffer};
}
