#include "integers.h" // IWYU pragma: keep

#include "clib/asserts.h"
#include "clib/platform.h"
#include "datatypes/bytes.h"

#define INTEGER_IMPL(T, BITS)                                                  \
  T T##_reverse_bits(T value) {                                                \
    T reversed = 0;                                                            \
    for (usize i = 0; i < BITS; i++) {                                         \
      reversed = reversed << 1;                                                \
      if (value & 1) {                                                         \
        reversed = reversed | 1;                                               \
      }                                                                        \
      value = value >> 1;                                                      \
    }                                                                          \
    return reversed;                                                           \
  }                                                                            \
                                                                               \
  T T##_swap_bytes(T value) {                                                  \
    if (1 < sizeof(T)) {                                                       \
      u8 *bytes = (u8 *)&value;                                                \
      for (size_t i = 0; i < (sizeof(T) + 1) / 2; i++) {                       \
        u8 temp = bytes[i];                                                    \
        bytes[i] = bytes[sizeof(T) - i - 1];                                   \
        bytes[sizeof(T) - i - 1] = temp;                                       \
      }                                                                        \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  usize T##_leading_ones(T value) {                                            \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (!(value >> (BITS - i - 1) & (T)0x1)) {                               \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_trailing_ones(T value) {                                           \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (!(value >> i & (T)0x1)) {                                            \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_leading_zeros(T value) {                                           \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (value >> (BITS - i - 1) & (T)0x1) {                                  \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_trailing_zeros(T value) {                                          \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (value >> i & (T)0x1) {                                               \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_count_zeros(T value) {                                             \
    usize count = 0;                                                           \
    for (size_t i = 0; i < BITS; i++) {                                        \
      if (!(value >> i & (T)0x1)) {                                            \
        count++;                                                               \
      }                                                                        \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_count_ones(T value) {                                              \
    usize count = 0;                                                           \
    for (size_t i = 0; i < BITS; i++) {                                        \
      if (value >> i & (T)0x1) {                                               \
        count++;                                                               \
      }                                                                        \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  T T##_to_be(T value) {                                                       \
    if (CLIB_BYTE_ORDER == ENDIAN_LITTLE) {                                    \
      return T##_swap_bytes(value);                                            \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  T T##_from_be(T value) {                                                     \
    if (CLIB_BYTE_ORDER == ENDIAN_LITTLE) {                                    \
      return T##_swap_bytes(value);                                            \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  T T##_from_be_bytes(Bytes bytes) {                                           \
    clib_assert(sizeof(T) == bytes.size, "Byte array correct size");           \
    if (CLIB_BYTE_ORDER == ENDIAN_LITTLE) {                                    \
      return T##_swap_bytes(*(T *)bytes.data);                                 \
    }                                                                          \
    return *(T *)bytes.data;                                                   \
  }                                                                            \
                                                                               \
  Bytes T##_to_be_bytes(T value, Arena *arena) {                               \
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
  }                                                                            \
                                                                               \
  T T##_to_le(T value) {                                                       \
    if (CLIB_BYTE_ORDER == ENDIAN_BIG) {                                       \
      return T##_swap_bytes(value);                                            \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  T T##_from_le(T value) {                                                     \
    if (CLIB_BYTE_ORDER == ENDIAN_BIG) {                                       \
      return T##_swap_bytes(value);                                            \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  T T##_from_le_bytes(Bytes bytes) {                                           \
    clib_assert(sizeof(T) == bytes.size, "Byte array not correct size");       \
    if (CLIB_BYTE_ORDER == ENDIAN_BIG) {                                       \
      return T##_swap_bytes(*(T *)bytes.data);                                 \
    }                                                                          \
    return *(T *)bytes.data;                                                   \
  }                                                                            \
                                                                               \
  Bytes T##_to_le_bytes(T value, Arena *arena) {                               \
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
  }                                                                            \
                                                                               \
  T T##_from_ne_bytes(Bytes bytes) {                                           \
    clib_assert(sizeof(T) == bytes.size, "Byte array not correct size");       \
    return *(T *)bytes.data;                                                   \
  }                                                                            \
                                                                               \
  Bytes T##_to_ne_bytes(T value, Arena *arena) {                               \
    if (CLIB_BYTE_ORDER == ENDIAN_BIG) {                                       \
      return T##_to_be_bytes(value, arena);                                    \
    }                                                                          \
    return T##_to_le_bytes(value, arena);                                      \
  }                                                                            \
                                                                               \
  T T##_max(T a, T b) { return a < b ? b : a; }                                \
  T T##_min(T a, T b) { return a > b ? b : a; }                                \
  T T##_clamp(T min, T max, T value) {                                         \
    return value < min ? min : max < value ? max : value;                      \
  }                                                                            \
                                                                               \
  CmpOrdering T##_compare_lt(T a, T b) {                                       \
    return a == b ? CMP_EQUAL : a < b ? CMP_LESS : CMP_GREATER;                \
  }                                                                            \
                                                                               \
  CmpOrdering T##_compare_gt(T a, T b) {                                       \
    return a == b ? CMP_EQUAL : a > b ? CMP_LESS : CMP_GREATER;                \
  }                                                                            \
                                                                               \
  static CmpOrdering _##T##_cmp_gt(const void *a, const void *b) {             \
    return T##_compare_gt(*(T *)a, *(T *)b);                                   \
  }                                                                            \
                                                                               \
  static CmpOrdering _##T##_cmp_lt(const void *a, const void *b) {             \
    return T##_compare_lt(*(T *)a, *(T *)b);                                   \
  }                                                                            \
                                                                               \
  CompareFn T##_compare_qsort(CmpOrdering ordering) {                          \
    return ordering == CMP_LESS ? _##T##_cmp_lt : _##T##_cmp_gt;               \
  }

INTEGER_IMPL(u8, U8_BITS)
INTEGER_IMPL(i8, I8_BITS)
INTEGER_IMPL(u16, U16_BITS)
INTEGER_IMPL(i16, I16_BITS)
INTEGER_IMPL(u32, U32_BITS)
INTEGER_IMPL(i32, I32_BITS)
INTEGER_IMPL(u64, U64_BITS)
INTEGER_IMPL(i64, I64_BITS)
INTEGER_IMPL(usize, USIZE_BITS)
