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
    for (size_t i = 0; i < sizeof(value) / 2; i++) {                           \
      u8 temp = bytes[i];                                                      \
      bytes[i] = bytes[sizeof(value) - i - 1];                                 \
      bytes[sizeof(value) - i - 1] = temp;                                     \
    }                                                                          \
    return value;                                                              \
  } while (0)

#define BITS_LEADING_ONES(value, BITS)                                         \
  do {                                                                         \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (!(value & (1 << (BITS - i - 1)))) {                                  \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  } while (0)

#define BITS_TRAILING_ONES(value, BITS)                                        \
  do {                                                                         \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (!(value & (1 << i))) {                                               \
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
      if (value & (1 << (BITS - i - 1))) {                                     \
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
      if (value & (1 << i)) {                                                  \
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
      if (!(value & (1 << i))) {                                               \
        count++;                                                               \
      }                                                                        \
    }                                                                          \
    return count;                                                              \
  } while (0)

#define BITS_COUNT_ONES(value, BITS)                                           \
  do {                                                                         \
    usize count = 0;                                                           \
    for (size_t i = 0; i < BITS; i++) {                                        \
      if (value & (1 << i)) {                                                  \
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

/* u8 */
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
#if CLIB_BYTE_ORDER == ENDIAN_BIG
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
/* u8 */

/* i8 */
i8 i8_reverse_bits(i8 value) { BITS_REVERSE(i8, value, I8_BITS); }
i8 i8_swap_bytes(i8 value) { BYTES_SWAP(value); }
usize i8_leading_ones(i8 value) { BITS_LEADING_ONES(value, I8_BITS); }
usize i8_trailing_ones(i8 value) { BITS_TRAILING_ONES(value, I8_BITS); }
usize i8_leading_zeros(i8 value) { BITS_LEADING_ZEROS(value, I8_BITS); }
usize i8_trailing_zeros(i8 value) { BITS_TRAILING_ZEROS(value, I8_BITS); }
usize i8_count_zeros(i8 value) { BITS_COUNT_ZEROS(value, I8_BITS); }
usize i8_count_ones(i8 value) { BITS_COUNT_ONES(value, I8_BITS); }

i8 i8_to_be(i8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return i8_swap_bytes(value);
#else
  return value;
#endif
}

i8 i8_from_be(i8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return i8_swap_bytes(value);
#else
  return value;
#endif
}

i8 i8_from_be_bytes(Bytes bytes) {
  clib_assert(sizeof(i8) == bytes.size, "Byte array correct size");
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return i8_swap_bytes(*(i8 *)bytes.data);
#else
  return *(i8 *)bytes.data;
#endif
}
Bytes i8_to_be_bytes(i8 value, Arena *arena) { TO_BE_BYTES(value, arena); }

i8 i8_to_le(i8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return i8_swap_bytes(value);
#else
  return value;
#endif
}

i8 i8_from_le(i8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return i8_swap_bytes(value);
#else
  return value;
#endif
}

i8 i8_from_le_bytes(Bytes bytes) {
  clib_assert(sizeof(i8) == bytes.size, "Byte array not correct size");
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return i8_swap_bytes(*(i8 *)bytes.data);
#else
  return *(i8 *)bytes.data;
#endif
}
Bytes i8_to_le_bytes(i8 value, Arena *arena) { TO_LE_BYTES(value, arena); }

i8 i8_from_ne_bytes(Bytes bytes) {
  clib_assert(sizeof(i8) == bytes.size, "Byte array not correct size");
  return *(i8 *)bytes.data;
}

Bytes i8_to_ne_bytes(i8 value, Arena *arena) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  TO_BE_BYTES(value, arena);
#else
  TO_LE_BYTES(value, arena);
#endif
}
/* i8 */

/* u16 */
u16 u16_reverse_bits(u16 value) { BITS_REVERSE(u16, value, U16_BITS); }
u16 u16_swap_bytes(u16 value) { BYTES_SWAP(value); }
usize u16_leading_ones(u16 value) { BITS_LEADING_ONES(value, U16_BITS); }
usize u16_trailing_ones(u16 value) { BITS_TRAILING_ONES(value, U16_BITS); }
usize u16_leading_zeros(u16 value) { BITS_LEADING_ZEROS(value, U16_BITS); }
usize u16_trailing_zeros(u16 value) { BITS_TRAILING_ZEROS(value, U16_BITS); }
usize u16_count_zeros(u16 value) { BITS_COUNT_ZEROS(value, U16_BITS); }
usize u16_count_ones(u16 value) { BITS_COUNT_ONES(value, U16_BITS); }

u16 u16_to_be(u16 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u16_swap_bytes(value);
#else
  return value;
#endif
}

u16 u16_from_be(u16 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u16_swap_bytes(value);
#else
  return value;
#endif
}

u16 u16_from_be_bytes(Bytes bytes) {
  clib_assert(sizeof(u16) == bytes.size, "Byte array correct size");
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u16_swap_bytes(*(u16 *)bytes.data);
#else
  return *(u16 *)bytes.data;
#endif
}
Bytes u16_to_be_bytes(u16 value, Arena *arena) { TO_BE_BYTES(value, arena); }

u16 u16_to_le(u16 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u16_swap_bytes(value);
#else
  return value;
#endif
}

u16 u16_from_le(u16 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u16_swap_bytes(value);
#else
  return value;
#endif
}

u16 u16_from_le_bytes(Bytes bytes) {
  clib_assert(sizeof(u16) == bytes.size, "Byte array not correct size");
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u16_swap_bytes(*(u16 *)bytes.data);
#else
  return *(u16 *)bytes.data;
#endif
}
Bytes u16_to_le_bytes(u16 value, Arena *arena) { TO_LE_BYTES(value, arena); }

u16 u16_from_ne_bytes(Bytes bytes) {
  clib_assert(sizeof(u16) == bytes.size, "Byte array not correct size");
  return *(u16 *)bytes.data;
}

Bytes u16_to_ne_bytes(u16 value, Arena *arena) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  TO_BE_BYTES(value, arena);
#else
  TO_LE_BYTES(value, arena);
#endif
}
/* u16 */

/* i16 */
i16 i16_reverse_bits(i16 value) { BITS_REVERSE(i16, value, I16_BITS); }
i16 i16_swap_bytes(i16 value) { BYTES_SWAP(value); }
usize i16_leading_ones(i16 value) { BITS_LEADING_ONES(value, I16_BITS); }
usize i16_trailing_ones(i16 value) { BITS_TRAILING_ONES(value, I16_BITS); }
usize i16_leading_zeros(i16 value) { BITS_LEADING_ZEROS(value, I16_BITS); }
usize i16_trailing_zeros(i16 value) { BITS_TRAILING_ZEROS(value, I16_BITS); }
usize i16_count_zeros(i16 value) { BITS_COUNT_ZEROS(value, I16_BITS); }
usize i16_count_ones(i16 value) { BITS_COUNT_ONES(value, I16_BITS); }

i16 i16_to_be(i16 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return i16_swap_bytes(value);
#else
  return value;
#endif
}

i16 i16_from_be(i16 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return i16_swap_bytes(value);
#else
  return value;
#endif
}

i16 i16_from_be_bytes(Bytes bytes) {
  clib_assert(sizeof(i16) == bytes.size, "Byte array correct size");
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return i16_swap_bytes(*(i16 *)bytes.data);
#else
  return *(i16 *)bytes.data;
#endif
}
Bytes i16_to_be_bytes(i16 value, Arena *arena) { TO_BE_BYTES(value, arena); }

i16 i16_to_le(i16 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return i16_swap_bytes(value);
#else
  return value;
#endif
}

i16 i16_from_le(i16 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return i16_swap_bytes(value);
#else
  return value;
#endif
}

i16 i16_from_le_bytes(Bytes bytes) {
  clib_assert(sizeof(i16) == bytes.size, "Byte array not correct size");
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return i16_swap_bytes(*(i16 *)bytes.data);
#else
  return *(i16 *)bytes.data;
#endif
}
Bytes i16_to_le_bytes(i16 value, Arena *arena) { TO_LE_BYTES(value, arena); }

i16 i16_from_ne_bytes(Bytes bytes) {
  clib_assert(sizeof(i16) == bytes.size, "Byte array not correct size");
  return *(i16 *)bytes.data;
}

Bytes i16_to_ne_bytes(i16 value, Arena *arena) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  TO_BE_BYTES(value, arena);
#else
  TO_LE_BYTES(value, arena);
#endif
}
/* i16 */

/* u32 */
u32 u32_reverse_bits(u32 value) { BITS_REVERSE(u32, value, U32_BITS); }
u32 u32_swap_bytes(u32 value) { BYTES_SWAP(value); }
usize u32_leading_ones(u32 value) { BITS_LEADING_ONES(value, U32_BITS); }
usize u32_trailing_ones(u32 value) { BITS_TRAILING_ONES(value, U32_BITS); }
usize u32_leading_zeros(u32 value) { BITS_LEADING_ZEROS(value, U32_BITS); }
usize u32_trailing_zeros(u32 value) { BITS_TRAILING_ZEROS(value, U32_BITS); }
usize u32_count_zeros(u32 value) { BITS_COUNT_ZEROS(value, U32_BITS); }
usize u32_count_ones(u32 value) { BITS_COUNT_ONES(value, U32_BITS); }

u32 u32_to_be(u32 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u32_swap_bytes(value);
#else
  return value;
#endif
}

u32 u32_from_be(u32 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u32_swap_bytes(value);
#else
  return value;
#endif
}

u32 u32_from_be_bytes(Bytes bytes) {
  clib_assert(sizeof(u32) == bytes.size, "Byte array correct size");
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u32_swap_bytes(*(u32 *)bytes.data);
#else
  return *(u32 *)bytes.data;
#endif
}
Bytes u32_to_be_bytes(u32 value, Arena *arena) { TO_BE_BYTES(value, arena); }

u32 u32_to_le(u32 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u32_swap_bytes(value);
#else
  return value;
#endif
}

u32 u32_from_le(u32 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u32_swap_bytes(value);
#else
  return value;
#endif
}

u32 u32_from_le_bytes(Bytes bytes) {
  clib_assert(sizeof(u32) == bytes.size, "Byte array not correct size");
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u32_swap_bytes(*(u32 *)bytes.data);
#else
  return *(u32 *)bytes.data;
#endif
}
Bytes u32_to_le_bytes(u32 value, Arena *arena) { TO_LE_BYTES(value, arena); }

u32 u32_from_ne_bytes(Bytes bytes) {
  clib_assert(sizeof(u32) == bytes.size, "Byte array not correct size");
  return *(u32 *)bytes.data;
}

Bytes u32_to_ne_bytes(u32 value, Arena *arena) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  TO_BE_BYTES(value, arena);
#else
  TO_LE_BYTES(value, arena);
#endif
}
/* u32 */
