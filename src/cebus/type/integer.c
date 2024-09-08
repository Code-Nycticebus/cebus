#include "integer.h" // IWYU pragma: keep

#include "cebus/core/debug.h"
#include "cebus/core/platform.h"
#include "cebus/type/byte.h"

#define BITS(T) (sizeof(T) * 8)

#define INTEGER_IMPL(T)                                                                            \
  /* BIT OPERATIONS */                                                                             \
  T T##_reverse_bits(T value) {                                                                    \
    T reversed = 0;                                                                                \
    for (usize i = 0; i < BITS(T); i++) {                                                          \
      reversed = (T)(reversed << 1);                                                               \
      if (value & 1) {                                                                             \
        reversed = reversed | 1;                                                                   \
      }                                                                                            \
      value = value >> 1;                                                                          \
    }                                                                                              \
    return reversed;                                                                               \
  }                                                                                                \
                                                                                                   \
  usize T##_leading_ones(T value) {                                                                \
    usize count = 0;                                                                               \
    for (usize i = 0; i < BITS(T); i++) {                                                          \
      if (!(value >> (BITS(T) - i - 1) & (T)0x1)) {                                                \
        break;                                                                                     \
      }                                                                                            \
      count++;                                                                                     \
    }                                                                                              \
    return count;                                                                                  \
  }                                                                                                \
                                                                                                   \
  usize T##_trailing_ones(T value) {                                                               \
    usize count = 0;                                                                               \
    for (usize i = 0; i < BITS(T); i++) {                                                          \
      if (!(value >> i & (T)0x1)) {                                                                \
        break;                                                                                     \
      }                                                                                            \
      count++;                                                                                     \
    }                                                                                              \
    return count;                                                                                  \
  }                                                                                                \
                                                                                                   \
  usize T##_leading_zeros(T value) {                                                               \
    usize count = 0;                                                                               \
    for (usize i = 0; i < BITS(T); i++) {                                                          \
      if (value >> (BITS(T) - i - 1) & (T)0x1) {                                                   \
        break;                                                                                     \
      }                                                                                            \
      count++;                                                                                     \
    }                                                                                              \
    return count;                                                                                  \
  }                                                                                                \
                                                                                                   \
  usize T##_trailing_zeros(T value) {                                                              \
    usize count = 0;                                                                               \
    for (usize i = 0; i < BITS(T); i++) {                                                          \
      if (value >> i & (T)0x1) {                                                                   \
        break;                                                                                     \
      }                                                                                            \
      count++;                                                                                     \
    }                                                                                              \
    return count;                                                                                  \
  }                                                                                                \
                                                                                                   \
  usize T##_count_zeros(T value) {                                                                 \
    usize count = 0;                                                                               \
    for (usize i = 0; i < BITS(T); i++) {                                                          \
      if (!(value >> i & (T)0x1)) {                                                                \
        count++;                                                                                   \
      }                                                                                            \
    }                                                                                              \
    return count;                                                                                  \
  }                                                                                                \
                                                                                                   \
  usize T##_count_ones(T value) {                                                                  \
    usize count = 0;                                                                               \
    for (usize i = 0; i < BITS(T); i++) {                                                          \
      if (value >> i & (T)0x1) {                                                                   \
        count++;                                                                                   \
      }                                                                                            \
    }                                                                                              \
    return count;                                                                                  \
  }                                                                                                \
  /* BIT OPERATIONS END */                                                                         \
                                                                                                   \
  /* BYTE ORDER OPERATIONS */                                                                      \
  T T##_swap_bytes(T value) {                                                                      \
    if (1 < sizeof(T)) {                                                                           \
      u8 *bytes = (u8 *)&value;                                                                    \
      for (usize i = 0; i < (sizeof(T) + 1) / 2; i++) {                                            \
        u8 temp = bytes[i];                                                                        \
        bytes[i] = bytes[sizeof(T) - i - 1];                                                       \
        bytes[sizeof(T) - i - 1] = temp;                                                           \
      }                                                                                            \
    }                                                                                              \
    return value;                                                                                  \
  }                                                                                                \
                                                                                                   \
  /*  BIG ENDIAN OPERATIONS */                                                                     \
  T T##_to_be(T value) {                                                                           \
    if (CEBUS_BYTE_ORDER == ENDIAN_LITTLE) {                                                       \
      return T##_swap_bytes(value);                                                                \
    }                                                                                              \
    return value;                                                                                  \
  }                                                                                                \
                                                                                                   \
  T T##_from_be(T value) {                                                                         \
    if (CEBUS_BYTE_ORDER == ENDIAN_LITTLE) {                                                       \
      return T##_swap_bytes(value);                                                                \
    }                                                                                              \
    return value;                                                                                  \
  }                                                                                                \
                                                                                                   \
  T T##_from_be_bytes(Bytes bytes) {                                                               \
    cebus_assert(sizeof(T) == bytes.size, "expected %" USIZE_FMT " bytes but got %" USIZE_FMT,     \
                 sizeof(T), bytes.size);                                                           \
    if (CEBUS_BYTE_ORDER == ENDIAN_LITTLE) {                                                       \
      return T##_swap_bytes(*(const T *)bytes.data);                                               \
    }                                                                                              \
    return *(const T *)bytes.data;                                                                 \
  }                                                                                                \
                                                                                                   \
  Bytes T##_to_be_bytes(T value, Arena *arena) {                                                   \
    u8 *buffer = arena_alloc(arena, sizeof(value));                                                \
    u8 *bytes = (u8 *)&value;                                                                      \
    for (usize i = 0; i < sizeof(value); i++) {                                                    \
      if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                                        \
        buffer[i] = bytes[i];                                                                      \
      } else {                                                                                     \
        buffer[sizeof(value) - i - 1] = bytes[i];                                                  \
      }                                                                                            \
    }                                                                                              \
    return bytes_from_parts(sizeof(value), buffer);                                                \
  }                                                                                                \
                                                                                                   \
  /*  LITTLE ENDIAN OPERATIONS */                                                                  \
  T T##_to_le(T value) {                                                                           \
    if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                                          \
      return T##_swap_bytes(value);                                                                \
    }                                                                                              \
    return value;                                                                                  \
  }                                                                                                \
                                                                                                   \
  T T##_from_le(T value) {                                                                         \
    if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                                          \
      return T##_swap_bytes(value);                                                                \
    }                                                                                              \
    return value;                                                                                  \
  }                                                                                                \
                                                                                                   \
  T T##_from_le_bytes(Bytes bytes) {                                                               \
    cebus_assert(sizeof(T) == bytes.size, "expected %" USIZE_FMT " bytes but got %" USIZE_FMT,     \
                 sizeof(T), bytes.size);                                                           \
    if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                                          \
      return T##_swap_bytes(*(const T *)bytes.data);                                               \
    }                                                                                              \
    return *(const T *)bytes.data;                                                                 \
  }                                                                                                \
                                                                                                   \
  Bytes T##_to_le_bytes(T value, Arena *arena) {                                                   \
    u8 *buffer = arena_alloc(arena, sizeof(value));                                                \
    u8 *bytes = (u8 *)&value;                                                                      \
    for (usize i = 0; i < sizeof(value); i++) {                                                    \
      if (CEBUS_BYTE_ORDER == ENDIAN_LITTLE) {                                                     \
        buffer[i] = bytes[i];                                                                      \
      } else {                                                                                     \
        buffer[sizeof(value) - i - 1] = bytes[i];                                                  \
      }                                                                                            \
    }                                                                                              \
    return bytes_from_parts(sizeof(value), buffer);                                                \
  }                                                                                                \
                                                                                                   \
  /*  NATIVE ENDIAN OPERATIONS */                                                                  \
  T T##_from_ne_bytes(Bytes bytes) {                                                               \
    cebus_assert(sizeof(T) == bytes.size, "expected %" USIZE_FMT " bytes but got %" USIZE_FMT,     \
                 sizeof(T), bytes.size);                                                           \
    return *(const T *)bytes.data;                                                                 \
  }                                                                                                \
                                                                                                   \
  Bytes T##_to_ne_bytes(T value, Arena *arena) {                                                   \
    if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                                          \
      return T##_to_be_bytes(value, arena);                                                        \
    }                                                                                              \
    return T##_to_le_bytes(value, arena);                                                          \
  }                                                                                                \
  /* BYTE ORDER OPERATIONS END */                                                                  \
                                                                                                   \
  /* MATH OPERATIONS */                                                                            \
  T T##_max(T a, T b) { return a < b ? b : a; }                                                    \
  T T##_min(T a, T b) { return a > b ? b : a; }                                                    \
  T T##_clamp(T min, T max, T value) { return value < min ? min : max < value ? max : value; }     \
  /* MATH OPERATIONS END */                                                                        \
                                                                                                   \
  /* UTILS */                                                                                      \
  u64 T##_hash(T value) {                                                                          \
    u64 hash = ((u64)value) + 1;                                                                   \
    hash = (((hash >> 16) ^ hash) % 0x3AA387A8B1) * 0x45d9f3b;                                     \
    hash = (((hash >> 16) ^ hash) % 0x3AA387A8B1) * 0x45d9f3b;                                     \
    hash = (hash >> 16) ^ hash;                                                                    \
    return hash;                                                                                   \
  }                                                                                                \
                                                                                                   \
  void T##_swap(T *v1, T *v2) {                                                                    \
    T temp = *v1;                                                                                  \
    *v1 = *v2;                                                                                     \
    *v2 = temp;                                                                                    \
  }                                                                                                \
                                                                                                   \
  CmpOrdering T##_compare_lt(T a, T b) {                                                           \
    return a == b ? CMP_EQUAL : a < b ? CMP_LESS : CMP_GREATER;                                    \
  }                                                                                                \
                                                                                                   \
  CmpOrdering T##_compare_gt(T a, T b) {                                                           \
    return a == b ? CMP_EQUAL : a > b ? CMP_LESS : CMP_GREATER;                                    \
  }                                                                                                \
                                                                                                   \
  static CmpOrdering _##T##_cmp_gt(const void *a, const void *b) {                                 \
    return T##_compare_gt(*(const T *)a, *(const T *)b);                                           \
  }                                                                                                \
                                                                                                   \
  static CmpOrdering _##T##_cmp_lt(const void *a, const void *b) {                                 \
    return T##_compare_lt(*(const T *)a, *(const T *)b);                                           \
  }                                                                                                \
                                                                                                   \
  CompareFn T##_compare_qsort(CmpOrdering ordering) {                                              \
    return ordering == CMP_LESS ? _##T##_cmp_lt : _##T##_cmp_gt;                                   \
  } /* UTILS END */

INTEGER_IMPL(u8)
INTEGER_IMPL(i8)
INTEGER_IMPL(u16)
INTEGER_IMPL(i16)
INTEGER_IMPL(u32)
INTEGER_IMPL(i32)
INTEGER_IMPL(u64)
INTEGER_IMPL(i64)
INTEGER_IMPL(usize)

#undef BITS
