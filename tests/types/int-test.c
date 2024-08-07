#include "cebus/type/integer.h"

#include "cebus/core/debug.h"
#include "cebus/type/byte.h"

/* u8 */
static void test_u8_leading_bits(void) {
  cebus_assert(u8_leading_ones(0xe0) == 3, "Did not count correctly");
  cebus_assert(u8_leading_zeros(0x1f) == 3, "Did not count correctly");
  cebus_assert(u8_trailing_ones(0x07) == 3, "Did not count correctly");
  cebus_assert(u8_trailing_zeros(0x08) == 3, "Did not count correctly");
}

static void test_u8_swaping_bits(void) {
  cebus_assert(u8_reverse_bits(0x12) == 0x48, "Did not reverse correctly");
  cebus_assert(u8_swap_bytes(0x12) == 0x12, "Did not swap correctly");
}

static void test_u8_endian(void) {
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(u8_to_le(0x12) == 0x12, "Bytes are somehow different");
  cebus_assert(u8_to_be(0x12) == 0x12, "Bytes are somehow different");
#else
  cebus_assert(u8_to_le(0x12) == 0x12, "Bytes are somehow different");
  cebus_assert(u8_to_be(0x12) == 0x12, "Bytes are somehow different");
#endif
}

static void test_u8_count_bits(void) {
  cebus_assert(u8_count_ones(0x12) == 2, "Did count correctly");
  cebus_assert(u8_count_zeros(0x12) == 6, "Did count correctly");
}

static void test_u8_from_bytes(void) {
  cebus_assert(u8_from_be_bytes(BYTES(0x12)) == 0x12, "Conversion not correct");
  cebus_assert(u8_from_le_bytes(BYTES(0x12)) == 0x12, "Conversion not correct");
  cebus_assert(u8_from_ne_bytes(BYTES(0x12)) == 0x12, "Conversion not correct");
}

static void test_u8_to_bytes(void) {
  Arena arena = {0};
  cebus_assert(bytes_eq(u8_to_be_bytes(0x12, &arena), BYTES(0x12)), "Not converted correctly");
  cebus_assert(bytes_eq(u8_to_le_bytes(0x12, &arena), BYTES(0x12)), "Not converted correctly");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(bytes_eq(u8_to_ne_bytes(0x12, &arena), BYTES(0x12)), "Not converted correctly");
#else
  cebus_assert(bytes_eq(u8_to_ne_bytes(0x12, &arena), BYTES(0x12)), "Not converted correctly");
#endif
  arena_free(&arena);
}

static void test_u8_hash(void) {
  cebus_assert(u8_hash(0) == 0x130f353e031ba7, "0x%" U64_HEX, u8_hash(0));
  cebus_assert(u8_hash(69) == 0x53620494cc400a2, "0x%" U64_HEX, u8_hash(69));
  cebus_assert(u8_hash(42) == 0x333883cd70fb570, "0x%" U64_HEX, u8_hash(42));
}
/* u8 */

/* i8 */
static void test_i8_leading_bits(void) {
  cebus_assert(i8_leading_ones((i8)0xe0) == 3, "Did not count correctly");
  cebus_assert(i8_leading_zeros((i8)0x1f) == 3, "Did not count correctly");
  cebus_assert(i8_trailing_ones((i8)0x07) == 3, "Did not count correctly");
  cebus_assert(i8_trailing_zeros((i8)0x08) == 3, "Did not count correctly");
}

static void test_i8_swaping_bits(void) {
  cebus_assert(i8_reverse_bits((i8)0x12) == 0x48, "Did not reverse correctly");
  cebus_assert(i8_swap_bytes((i8)0x12) == 0x12, "Did not swap correctly");
}

static void test_i8_endian(void) {
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(i8_to_le((i8)0x1a) == 0x1a, "Bytes are somehow different");
  cebus_assert(i8_to_be((i8)0x1a) == 0x1a, "Bytes are somehow different");
#else
  cebus_assert(i8_to_le((i8)0x1a) == 0x1a, "Bytes are somehow different");
  cebus_assert(i8_to_be((i8)0x1a) == 0x1a, "Bytes are somehow different");
#endif
}

static void test_i8_count_bits(void) {
  cebus_assert(i8_count_ones((i8)0x12) == 2, "Did count correctly");
  cebus_assert(i8_count_zeros((i8)0x12) == 6, "Did count correctly");
}

static void test_i8_from_bytes(void) {
  cebus_assert(i8_from_be_bytes(BYTES(0x12)) == 0x12, "Conversion not correct");
  cebus_assert(i8_from_le_bytes(BYTES(0x12)) == 0x12, "Conversion not correct");
  cebus_assert(i8_from_ne_bytes(BYTES(0x12)) == 0x12, "Conversion not correct");
}

static void test_i8_to_bytes(void) {
  Arena arena = {0};
  cebus_assert(bytes_eq(i8_to_be_bytes((i8)0x12, &arena), BYTES(0x12)), "Not converted correctly");
  cebus_assert(bytes_eq(i8_to_le_bytes((i8)0x12, &arena), BYTES(0x12)), "Not converted correctly");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(bytes_eq(i8_to_ne_bytes(0x12, &arena), BYTES(0x12)), "Not converted correctly");
#else
  cebus_assert(bytes_eq(i8_to_ne_bytes((i8)0x12, &arena), BYTES(0x12)), "Not converted correctly");
#endif
  arena_free(&arena);
}

static void test_i8_hash(void) {
  cebus_assert(i8_hash(0) == 0x130f353e031ba7, "0x%" U64_HEX, i8_hash(0));
  cebus_assert(i8_hash(69) == 0x53620494cc400a2, "0x%" U64_HEX, i8_hash(69));
  cebus_assert(i8_hash(-69) == 0xe8a748836a8337d1, "0x%" U64_HEX, i8_hash(-69));
  cebus_assert(i8_hash(42) == 0x333883cd70fb570, "0x%" U64_HEX, i8_hash(42));
}
/* i8 */

/* u16 */
static void test_u16_leading_bits(void) {
  cebus_assert(u16_leading_ones(0xe001) == 3, "Did not count correctly");
  cebus_assert(u16_leading_zeros(0x1f00) == 3, "Did not count correctly");
  cebus_assert(u16_trailing_ones(0x1007) == 3, "Did not count correctly");
  cebus_assert(u16_trailing_zeros(0x1008) == 3, "Did not count correctly");
}

static void test_u16_swaping_bits(void) {
  cebus_assert(u16_reverse_bits(0x1234) == 0x2c48, "Did not reverse correctly");
  cebus_assert(u16_swap_bytes(0x1234) == 0x3412, "Did not swap correctly");
}

static void test_u16_endian(void) {
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(u16_to_le(0x1234) == 0x3412, "Bytes are somehow different");
  cebus_assert(u16_to_be(0x1234) == 0x1234, "Bytes are somehow different");
#else
  cebus_assert(u16_to_le(0x1234) == 0x1234, "Bytes are somehow different");
  cebus_assert(u16_to_be(0x1234) == 0x3412, "Bytes are somehow different");
#endif
}

static void test_u16_count_bits(void) {
  cebus_assert(u16_count_ones(0x1234) == 5, "Did count correctly");
  cebus_assert(u16_count_zeros(0x1234) == 11, "Did count correctly");
}

static void test_u16_from_bytes(void) {
  cebus_assert(u16_from_be_bytes(BYTES(0x12, 0x34)) == 0x1234, "Conversion not correct");
  cebus_assert(u16_from_le_bytes(BYTES(0x34, 0x12)) == 0x1234, "Conversion not correct");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(u16_from_ne_bytes(BYTES(0x12, 0x34)) == 0x1234, "Not converted correctly");
#else
  cebus_assert(u16_from_ne_bytes(BYTES(0x34, 0x12)) == 0x1234, "Not converted correctly");
#endif
}

static void test_u16_to_bytes(void) {
  Arena arena = {0};
  cebus_assert(bytes_eq(u16_to_be_bytes(0x1234, &arena), BYTES(0x12, 0x34)),
               "Not converted correctly");
  cebus_assert(bytes_eq(u16_to_le_bytes(0x1234, &arena), BYTES(0x34, 0x12)),
               "Not converted correctly");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(bytes_eq(u16_to_ne_bytes(0x1234, &arena), BYTES(0x12, 0x34)),
               "Not converted correctly");
#else
  cebus_assert(bytes_eq(u16_to_ne_bytes(0x1234, &arena), BYTES(0x34, 0x12)),
               "Not converted correctly");
#endif
  arena_free(&arena);
}

static void test_u16_hash(void) {
  cebus_assert(u16_hash(0) == 0x130f353e031ba7, "0x%" U64_HEX, u16_hash(0));
  cebus_assert(u16_hash(69) == 0x53620494cc400a2, "0x%" U64_HEX, u16_hash(69));
  cebus_assert(u16_hash(42) == 0x333883cd70fb570, "0x%" U64_HEX, u16_hash(42));
}
/* u16 */

/* i16 */
static void test_i16_leading_bits(void) {
  cebus_assert(i16_leading_ones((i16)0xe001) == 3, "Did not count correctly");
  cebus_assert(i16_leading_zeros((i16)0x1f00) == 3, "Did not count correctly");
  cebus_assert(i16_trailing_ones((i16)0x1007) == 3, "Did not count correctly");
  cebus_assert(i16_trailing_zeros((i16)0x1008) == 3, "Did not count correctly");
}

static void test_i16_swaping_bits(void) {
  cebus_assert(i16_reverse_bits(0x1234) == 0x2c48, "Did not reverse correctly");
  cebus_assert(i16_swap_bytes(0x1234) == 0x3412, "Did not swap correctly");
}

static void test_i16_endian(void) {
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(i16_to_le(0x1234) == 0x3412, "Bytes are somehow different");
  cebus_assert(i16_to_be(0x1234) == 0x1234, "Bytes are somehow different");
#else
  cebus_assert(i16_to_le(0x1234) == 0x1234, "Bytes are somehow different");
  cebus_assert(i16_to_be(0x1234) == 0x3412, "Bytes are somehow different");
#endif
}

static void test_i16_count_bits(void) {
  cebus_assert(i16_count_ones(0x1234) == 5, "Did count correctly");
  cebus_assert(i16_count_zeros(0x1234) == 11, "Did count correctly");
}

static void test_i16_from_bytes(void) {
  cebus_assert(i16_from_be_bytes(BYTES(0x12, 0x34)) == 0x1234, "Conversion not correct");
  cebus_assert(i16_from_le_bytes(BYTES(0x34, 0x12)) == 0x1234, "Conversion not correct");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(i16_from_ne_bytes(BYTES(0x12, 0x34)) == 0x1234, "Not converted correctly");
#else
  cebus_assert(i16_from_ne_bytes(BYTES(0x34, 0x12)) == 0x1234, "Not converted correctly");
#endif
}

static void test_i16_to_bytes(void) {
  Arena arena = {0};
  cebus_assert(bytes_eq(i16_to_be_bytes(0x1234, &arena), BYTES(0x12, 0x34)),
               "Not converted correctly");
  cebus_assert(bytes_eq(i16_to_le_bytes(0x1234, &arena), BYTES(0x34, 0x12)),
               "Not converted correctly");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(bytes_eq(i16_to_ne_bytes(0x1234, &arena), BYTES(0x12, 0x34)),
               "Not converted correctly");
#else
  cebus_assert(bytes_eq(i16_to_ne_bytes(0x1234, &arena), BYTES(0x34, 0x12)),
               "Not converted correctly");
#endif
  arena_free(&arena);
}

static void test_i16_hash(void) {
  cebus_assert(i16_hash(0) == 0x130f353e031ba7, "0x%" U64_HEX, i16_hash(0));
  cebus_assert(i16_hash(69) == 0x53620494cc400a2, "0x%" U64_HEX, i16_hash(69));
  cebus_assert(i16_hash(42) == 0x333883cd70fb570, "0x%" U64_HEX, i16_hash(42));
}
/* i16 */

/* u32 */
static void test_u32_leading_bits(void) {
  cebus_assert(u32_leading_ones(0xe0000001) == 3, "Did not count correctly");
  cebus_assert(u32_leading_zeros(0x1f000000) == 3, "Did not count correctly");
  cebus_assert(u32_trailing_ones(0x80000007) == 3, "Did not count correctly");
  cebus_assert(u32_trailing_zeros(0x80000008) == 3, "Did not count correctly");
}

static void test_u32_swaping_bits(void) {
  cebus_assert(u32_reverse_bits(0x12345678) == 0x1e6a2c48, "Did not reverse correctly");
  cebus_assert(u32_swap_bytes(0x12345678) == 0x78563412, "Did not swap correctly");
}

static void test_u32_endian(void) {
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(u32_to_le(0x12345678) == 0x78563412, "Bytes are somehow different");
  cebus_assert(u32_to_be(0x12345678) == 0x12345678, "Bytes are somehow different");
#else
  cebus_assert(u32_to_le(0x12345678) == 0x12345678, "Bytes are somehow different");
  cebus_assert(u32_to_be(0x12345678) == 0x78563412, "Bytes are somehow different");
#endif
}

static void test_u32_count_bits(void) {
  cebus_assert(u32_count_ones(0x12345678) == 13, "Did count correctly");
  cebus_assert(u32_count_zeros(0x12345678) == 19, "Did count correctly");
}

static void test_u32_from_bytes(void) {
  cebus_assert(u32_from_be_bytes(BYTES(0x12, 0x34, 0x56, 0x78)) == 0x12345678,
               "Conversion not correct");
  cebus_assert(u32_from_le_bytes(BYTES(0x78, 0x56, 0x34, 0x12)) == 0x12345678,
               "Conversion not correct");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(u32_from_ne_bytes(BYTES(0x12, 0x34, 0x56, 0x78)) == 0x12345678,
               "Not converted correctly");
#else
  cebus_assert(u32_from_ne_bytes(BYTES(0x78, 0x56, 0x34, 0x12)) == 0x12345678,
               "Not converted correctly");
#endif
}

static void test_u32_to_bytes(void) {
  Arena arena = {0};
  cebus_assert(bytes_eq(u32_to_be_bytes(0x12345678, &arena), BYTES(0x12, 0x34, 0x56, 0x78)),
               "Not converted correctly");
  cebus_assert(bytes_eq(u32_to_le_bytes(0x12345678, &arena), BYTES(0x78, 0x56, 0x34, 0x12)),
               "Not converted correctly");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(bytes_eq(u32_to_ne_bytes(0x12345678, &arena), BYTES(0x12, 0x34, 0x56, 0x78)),
               "Not converted correctly");
#else
  cebus_assert(bytes_eq(u32_to_ne_bytes(0x12345678, &arena), BYTES(0x78, 0x56, 0x34, 0x12)),
               "Not converted correctly");
#endif
  arena_free(&arena);
}

static void test_u32_hash(void) {
  cebus_assert(u32_hash(0) == 0x130f353e031ba7, "0x%" U64_HEX, u32_hash(0));
  cebus_assert(u32_hash(69) == 0x53620494cc400a2, "0x%" U64_HEX, u32_hash(69));
  cebus_assert(u32_hash(42) == 0x333883cd70fb570, "0x%" U64_HEX, u32_hash(42));
}
/* u32 */

/* i32 */
static void test_i32_leading_bits(void) {
  cebus_assert(i32_leading_ones((i32)0xe0000001) == 3, "Did not count correctly");
  cebus_assert(i32_leading_zeros((i32)0x1f000000) == 3, "Did not count correctly");
  cebus_assert(i32_trailing_ones((i32)0x80000007) == 3, "Did not count correctly");
  cebus_assert(i32_trailing_zeros((i32)0x80000008) == 3, "Did not count correctly");
}

static void test_i32_swaping_bits(void) {
  cebus_assert(i32_reverse_bits(0x12345678) == 0x1e6a2c48, "Did not reverse correctly");
  cebus_assert(i32_swap_bytes(0x12345678) == 0x78563412, "Did not swap correctly");
}

static void test_i32_endian(void) {
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(i32_to_le(0x12345678) == 0x78563412, "Bytes are somehow different");
  cebus_assert(i32_to_be(0x12345678) == 0x12345678, "Bytes are somehow different");
#else
  cebus_assert(i32_to_le(0x12345678) == 0x12345678, "Bytes are somehow different");
  cebus_assert(i32_to_be(0x12345678) == 0x78563412, "Bytes are somehow different");
#endif
}

static void test_i32_count_bits(void) {
  cebus_assert(i32_count_ones(0x12345678) == 13, "Did count correctly");
  cebus_assert(i32_count_zeros(0x12345678) == 19, "Did count correctly");
}

static void test_i32_from_bytes(void) {
  cebus_assert(i32_from_be_bytes(BYTES(0x12, 0x34, 0x56, 0x78)) == 0x12345678,
               "Conversion not correct");
  cebus_assert(i32_from_le_bytes(BYTES(0x78, 0x56, 0x34, 0x12)) == 0x12345678,
               "Conversion not correct");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(i32_from_ne_bytes(BYTES(0x12, 0x34, 0x56, 0x78)) == 0x12345678,
               "Not converted correctly");
#else
  cebus_assert(i32_from_ne_bytes(BYTES(0x78, 0x56, 0x34, 0x12)) == 0x12345678,
               "Not converted correctly");
#endif
}

static void test_i32_to_bytes(void) {
  Arena arena = {0};
  cebus_assert(bytes_eq(i32_to_be_bytes(0x12345678, &arena), BYTES(0x12, 0x34, 0x56, 0x78)),
               "Not converted correctly");
  cebus_assert(bytes_eq(i32_to_le_bytes(0x12345678, &arena), BYTES(0x78, 0x56, 0x34, 0x12)),
               "Not converted correctly");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(bytes_eq(i32_to_ne_bytes(0x12345678, &arena), BYTES(0x12, 0x34, 0x56, 0x78)),
               "Not converted correctly");
#else
  cebus_assert(bytes_eq(i32_to_ne_bytes(0x12345678, &arena), BYTES(0x78, 0x56, 0x34, 0x12)),
               "Not converted correctly");
#endif
  arena_free(&arena);
}

static void test_i32_hash(void) {
  cebus_assert(i32_hash(0) == 0x130f353e031ba7, "0x%" U64_HEX, i32_hash(0));
  cebus_assert(i32_hash(69) == 0x53620494cc400a2, "0x%" U64_HEX, i32_hash(69));
  cebus_assert(i32_hash(42) == 0x333883cd70fb570, "0x%" U64_HEX, i32_hash(42));
}
/* i32 */

/* u64 */
static void test_u64_leading_bits(void) {
  cebus_assert(u64_leading_ones(0xe000000000000001ULL) == 3, "Did not count correctly");
  cebus_assert(u64_leading_zeros(0x1f00000000000000) == 3, "Did not count correctly");
  cebus_assert(u64_trailing_ones(0x8000000000000007) == 3, "Did not count correctly");
  cebus_assert(u64_trailing_zeros(0x8000000000000008) == 3, "Did not count correctly");
}

static void test_u64_swaping_bits(void) {
  cebus_assert(u64_reverse_bits(0x1234567812345678) == 0x1e6a2c481e6a2c48,
               "Did not reverse correctly");
  cebus_assert(u64_swap_bytes(0x1234567812345678) == 0x7856341278563412, "Did not swap correctly");
}

static void test_u64_endian(void) {
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(u64_to_le(0x1234567812345678) == 0x7856341278563412, "Bytes are somehow different");
  cebus_assert(u64_to_be(0x1234567812345678) == 0x1234567812345678, "Bytes are somehow different");
#else
  cebus_assert(u64_to_le(0x1234567812345678) == 0x1234567812345678, "Bytes are somehow different");
  cebus_assert(u64_to_be(0x1234567812345678) == 0x7856341278563412, "Bytes are somehow different");
#endif
}

static void test_u64_count_bits(void) {
  cebus_assert(u64_count_ones(0x1234567812345678) == 26, "Did count correctly");
  cebus_assert(u64_count_zeros(0x1234567812345678) == 38, "Did count correctly");
}

static void test_u64_from_bytes(void) {
  cebus_assert(u64_from_be_bytes(BYTES(0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78)) ==
                   0x1234567812345678,
               "Conversion not correct");
  cebus_assert(u64_from_le_bytes(BYTES(0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12)) ==
                   0x1234567812345678,
               "Conversion not correct");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(u64_from_ne_bytes(BYTES(0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78)) ==
                   0x1234567812345678,
               "Not converted correctly");
#else
  cebus_assert(u64_from_ne_bytes(BYTES(0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12)) ==
                   0x1234567812345678,
               "Not converted correctly");
#endif
}

static void test_u64_to_bytes(void) {
  Arena arena = {0};
  cebus_assert(bytes_eq(u64_to_be_bytes(0x1234567812345678, &arena),
                        BYTES(0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78)),
               "Not converted correctly");
  cebus_assert(bytes_eq(u64_to_le_bytes(0x1234567812345678, &arena),
                        BYTES(0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12)),
               "Not converted correctly");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(bytes_eq(u64_to_ne_bytes(0x1234567812345678, &arena),
                        BYTES(0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78)),
               "Not converted correctly");
#else
  cebus_assert(bytes_eq(u64_to_ne_bytes(0x1234567812345678, &arena),
                        BYTES(0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12)),
               "Not converted correctly");
#endif
  arena_free(&arena);
}

static void test_u64_hash(void) {
  cebus_assert(u64_hash(0) == 0x130f353e031ba7, "0x%" U64_HEX, u64_hash(0));
  cebus_assert(u64_hash(69) == 0x53620494cc400a2, "0x%" U64_HEX, u64_hash(69));
  cebus_assert(u64_hash(42) == 0x333883cd70fb570, "0x%" U64_HEX, u64_hash(42));
}
/* u64 */

/* i64 */
static void test_i64_leading_bits(void) {
  cebus_assert(i64_leading_ones((i64)0xe000000000000001LL) == 3, "Did not count correctly");
  cebus_assert(i64_leading_zeros((i64)0x1f00000000000000) == 3, "Did not count correctly");
  cebus_assert(i64_trailing_ones((i64)0x8000000000000007) == 3, "Did not count correctly");
  cebus_assert(i64_trailing_zeros((i64)0x8000000000000008) == 3, "Did not count correctly");
}

static void test_i64_swaping_bits(void) {
  cebus_assert(i64_reverse_bits(0x1234567812345678) == 0x1e6a2c481e6a2c48,
               "Did not reverse correctly");
  cebus_assert(i64_swap_bytes(0x1234567812345678) == 0x7856341278563412, "Did not swap correctly");
}

static void test_i64_endian(void) {
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(i64_to_le(0x1234567812345678) == 0x7856341278563412, "Bytes are somehow different");
  cebus_assert(i64_to_be(0x1234567812345678) == 0x1234567812345678, "Bytes are somehow different");
#else
  cebus_assert(i64_to_le(0x1234567812345678) == 0x1234567812345678, "Bytes are somehow different");
  cebus_assert(i64_to_be(0x1234567812345678) == 0x7856341278563412, "Bytes are somehow different");
#endif
}

static void test_i64_count_bits(void) {
  cebus_assert(i64_count_ones(0x1234567812345678) == 26, "Did count correctly");
  cebus_assert(i64_count_zeros(0x1234567812345678) == 38, "Did count correctly");
}

static void test_i64_from_bytes(void) {
  cebus_assert(i64_from_be_bytes(BYTES(0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78)) ==
                   0x1234567812345678,
               "Conversion not correct");
  cebus_assert(i64_from_le_bytes(BYTES(0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12)) ==
                   0x1234567812345678,
               "Conversion not correct");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(i64_from_ne_bytes(BYTES(0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78)) ==
                   0x1234567812345678,
               "Not converted correctly");
#else
  cebus_assert(i64_from_ne_bytes(BYTES(0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12)) ==
                   0x1234567812345678,
               "Not converted correctly");
#endif
}

static void test_i64_to_bytes(void) {
  Arena arena = {0};
  cebus_assert(bytes_eq(i64_to_be_bytes(0x1234567812345678, &arena),
                        BYTES(0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78)),
               "Not converted correctly");
  cebus_assert(bytes_eq(i64_to_le_bytes(0x1234567812345678, &arena),
                        BYTES(0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12)),
               "Not converted correctly");
#if CEBUS_BYTE_ORDER == ENDIAN_BIG
  cebus_assert(bytes_eq(i64_to_ne_bytes(0x1234567812345678, &arena),
                        BYTES(0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78)),
               "Not converted correctly");
#else
  cebus_assert(bytes_eq(i64_to_ne_bytes(0x1234567812345678, &arena),
                        BYTES(0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12)),
               "Not converted correctly");
#endif
  arena_free(&arena);
}

static void test_i64_hash(void) {
  cebus_assert(i64_hash(0) == 0x130f353e031ba7, "0x%" U64_HEX, i64_hash(0));
  cebus_assert(i64_hash(69) == 0x53620494cc400a2, "0x%" U64_HEX, i64_hash(69));
  cebus_assert(i64_hash(42) == 0x333883cd70fb570, "0x%" U64_HEX, i64_hash(42));
}
/* i64 */

int main(void) {
  test_u8_leading_bits();
  test_u8_swaping_bits();
  test_u8_endian();
  test_u8_count_bits();
  test_u8_from_bytes();
  test_u8_to_bytes();
  test_u8_hash();

  test_i8_leading_bits();
  test_i8_swaping_bits();
  test_i8_endian();
  test_i8_count_bits();
  test_i8_from_bytes();
  test_i8_to_bytes();
  test_i8_hash();

  test_u16_leading_bits();
  test_u16_swaping_bits();
  test_u16_endian();
  test_u16_count_bits();
  test_u16_from_bytes();
  test_u16_to_bytes();
  test_u16_hash();

  test_i16_leading_bits();
  test_i16_swaping_bits();
  test_i16_endian();
  test_i16_count_bits();
  test_i16_from_bytes();
  test_i16_to_bytes();
  test_i16_hash();

  test_u32_leading_bits();
  test_u32_swaping_bits();
  test_u32_endian();
  test_u32_count_bits();
  test_u32_from_bytes();
  test_u32_to_bytes();
  test_u32_hash();

  test_i32_leading_bits();
  test_i32_swaping_bits();
  test_i32_endian();
  test_i32_count_bits();
  test_i32_from_bytes();
  test_i32_to_bytes();
  test_i32_hash();

  test_u64_leading_bits();
  test_u64_swaping_bits();
  test_u64_endian();
  test_u64_count_bits();
  test_u64_from_bytes();
  test_u64_to_bytes();
  test_u64_hash();

  test_i64_leading_bits();
  test_i64_swaping_bits();
  test_i64_endian();
  test_i64_count_bits();
  test_i64_from_bytes();
  test_i64_to_bytes();
  test_i64_hash();
}
