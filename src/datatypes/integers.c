#include "integers.h"

#include "clib/asserts.h"
#include "clib/platform.h"
#include "datatypes/bytes.h"

#define BITS_REVERSE(T, value, BITS)                                           \
  do {                                                                         \
    T reversed = 0;                                                            \
    for (usize i = 0; i < BITS; i++) {                                         \
      reversed = reversed << 1;                                                \
      if (value & 1) {                                                         \
        reversed = reversed | 1;                                               \
      }                                                                        \
      value = value >> 1;                                                      \
    }                                                                          \
    return reversed;                                                           \
  } while (0)

#define BYTES_SWAP(value)                                                      \
  do {                                                                         \
    u8 *bytes = (u8 *)&value;                                                  \
    for (size_t i = 0; i < sizeof(value); i++) {                               \
      u8 temp = bytes[i];                                                      \
      bytes[i] = bytes[sizeof(value) - i - 1];                                 \
      bytes[sizeof(value) - i - 1] = temp;                                     \
    }                                                                          \
    return value;                                                              \
  } while (0)

#define BITS_LEADING_ONES(value, BITS)                                         \
  do {                                                                         \
    usize count = 0;                                                           \
    for (usize i = 0; i < U8_BITS; i++) {                                      \
      if (!(value & (0x80 >> i))) {                                            \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  } while (0)

#define BITS_TRAILING_ONES(value, BITS)                                        \
  do {                                                                         \
    usize count = 0;                                                           \
    for (usize i = 0; i < U8_BITS; i++) {                                      \
      if (!(value & (0x1 << i))) {                                             \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  } while (0)

#define BITS_LEADING_ZEROS(value, BITS)                                        \
  do {                                                                         \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (value & (0x80 >> i)) {                                               \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  } while (0)

#define BITS_TRAILING_ZEROS(value, BITS)                                       \
  do {                                                                         \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (value & (0x1 << i)) {                                                \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  } while (0)

#define BITS_COUNT_ZEROS(value, BITS)                                          \
  do {                                                                         \
    usize count = 0;                                                           \
    for (size_t i = 0; i < BITS; i++) {                                        \
      if (!(value & (0x80 >> i))) {                                            \
        count++;                                                               \
      }                                                                        \
    }                                                                          \
    return count;                                                              \
  } while (0)

#define BITS_COUNT_ONES(value, BITS)                                           \
  do {                                                                         \
    usize count = 0;                                                           \
    for (size_t i = 0; i < BITS; i++) {                                        \
      if (value & (0x80 >> i)) {                                               \
        count++;                                                               \
      }                                                                        \
    }                                                                          \
    return count;                                                              \
  } while (0)

#define TO_LE_BYTES(value, arena)                                              \
  do {                                                                         \
    u8 *buffer = arena_alloc(arena, sizeof(value));                            \
    u8 *bytes = (u8 *)&value;                                                  \
    for (size_t i = 0; i < sizeof(value); i++) {                               \
      if (CLIB_BYTE_ORDER == ENDIAN_LITTLE) {                                  \
        buffer[i] = bytes[i];                                                  \
      } else {                                                                 \
        buffer[sizeof(value) - i - 1] = bytes[i];                              \
      }                                                                        \
    }                                                                          \
    return bytes_from_parts(sizeof(value), buffer);                            \
  } while (0)

#define TO_BE_BYTES(value, arena)                                              \
  do {                                                                         \
    u8 *buffer = arena_alloc(arena, sizeof(value));                            \
    u8 *bytes = (u8 *)&value;                                                  \
    for (size_t i = 0; i < sizeof(value); i++) {                               \
      if (CLIB_BYTE_ORDER == ENDIAN_BIG) {                                     \
        buffer[i] = bytes[i];                                                  \
      } else {                                                                 \
        buffer[sizeof(value) - i - 1] = bytes[i];                              \
      }                                                                        \
    }                                                                          \
    return bytes_from_parts(sizeof(value), buffer);                            \
  } while (0)

u8 u8_reverse_bits(u8 value) { BITS_REVERSE(u8, value, U8_BITS); }
u8 u8_swap_bytes(u8 value) { BYTES_SWAP(value); }
usize u8_leading_ones(u8 value) { BITS_LEADING_ONES(value, U8_BITS); }
usize u8_trailing_ones(u8 value) { BITS_TRAILING_ONES(value, U8_BITS); }
usize u8_leading_zeros(u8 value) { BITS_LEADING_ZEROS(value, U8_BITS); }
usize u8_trailing_zeros(u8 value) { BITS_TRAILING_ZEROS(value, U8_BITS); }
usize u8_count_zeros(u8 value) { BITS_COUNT_ZEROS(value, U8_BITS); }
usize u8_count_ones(u8 value) { BITS_COUNT_ONES(value, U8_BITS); }

u8 u8_to_be(u8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u8_swap_bytes(value);
#else
  return value;
#endif
}

u8 u8_from_be(u8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u8_swap_bytes(value);
#else
  return value;
#endif
}

u8 u8_from_be_bytes(Bytes bytes) {
  clib_assert(sizeof(u8) == bytes.size, "Byte array correct size");
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u8_swap_bytes(*(u8 *)bytes.data);
#else
  return *(u8 *)bytes.data;
#endif
}
Bytes u8_to_be_bytes(u8 value, Arena *arena) { TO_BE_BYTES(value, arena); }

u8 u8_to_le(u8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u8_swap_bytes(value);
#else
  return value;
#endif
}

u8 u8_from_le(u8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u8_swap_bytes(value);
#else
  return value;
#endif
}

u8 u8_from_le_bytes(Bytes bytes) {
  clib_assert(sizeof(u8) == bytes.size, "Byte array not correct size");
#if CLIB_BYE_ORDER == ENDIAN_BIG
  return u8_swap_bytes(*(u8 *)bytes.data);
#else
  return *(u8 *)bytes.data;
#endif
}
Bytes u8_to_le_bytes(u8 value, Arena *arena) { TO_LE_BYTES(value, arena); }

u8 u8_from_ne_bytes(Bytes bytes) {
  clib_assert(sizeof(u8) == bytes.size, "Byte array not correct size");
  return *(u8 *)bytes.data;
}

Bytes u8_to_ne_bytes(u8 value, Arena *arena) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  TO_BE_BYTES(value, arena);
#else
  TO_LE_BYTES(value, arena);
#endif
}
