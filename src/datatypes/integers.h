#ifndef __CLIB_INTEGERS_H__
#define __CLIB_INTEGERS_H__

#include "clib/arena.h"
#include "datatypes.h"

/* u8 */
u8 u8_reverse_bits(u8 value);
u8 u8_swap_bytes(u8 value);
usize u8_leading_ones(u8 value);
usize u8_trailing_ones(u8 value);
usize u8_leading_zeros(u8 value);
usize u8_trailing_zeros(u8 value);
usize u8_count_zeros(u8 value);
usize u8_count_ones(u8 value);

u8 u8_to_be(u8 value);
u8 u8_from_be(u8 value);
u8 u8_from_be_bytes(Bytes bytes);
Bytes u8_to_be_bytes(u8 value, Arena *arena);

u8 u8_to_le(u8 value);
u8 u8_from_le(u8 value);
u8 u8_from_le_bytes(Bytes bytes);
Bytes u8_to_le_bytes(u8 value, Arena *arena);

u8 u8_from_ne_bytes(Bytes bytes);
Bytes u8_to_ne_bytes(u8 value, Arena *arena);
/* u8 */

/* i8 */
i8 i8_reverse_bits(i8 value);
i8 i8_swap_bytes(i8 value);
usize i8_leading_ones(i8 value);
usize i8_trailing_ones(i8 value);
usize i8_leading_zeros(i8 value);
usize i8_trailing_zeros(i8 value);
usize i8_count_zeros(i8 value);
usize i8_count_ones(i8 value);

i8 i8_to_be(i8 value);
i8 i8_from_be(i8 value);
i8 i8_from_be_bytes(Bytes bytes);
Bytes i8_to_be_bytes(i8 value, Arena *arena);

i8 i8_to_le(i8 value);
i8 i8_from_le(i8 value);
i8 i8_from_le_bytes(Bytes bytes);
Bytes i8_to_le_bytes(i8 value, Arena *arena);

i8 i8_from_ne_bytes(Bytes bytes);
Bytes i8_to_ne_bytes(i8 value, Arena *arena);
/* i8 */

/* u16 */
u16 u16_reverse_bits(u16 value);
u16 u16_swap_bytes(u16 value);
usize u16_leading_ones(u16 value);
usize u16_trailing_ones(u16 value);
usize u16_leading_zeros(u16 value);
usize u16_trailing_zeros(u16 value);
usize u16_count_zeros(u16 value);
usize u16_count_ones(u16 value);

u16 u16_to_be(u16 value);
u16 u16_from_be(u16 value);
u16 u16_from_be_bytes(Bytes bytes);
Bytes u16_to_be_bytes(u16 value, Arena *arena);

u16 u16_to_le(u16 value);
u16 u16_from_le(u16 value);
u16 u16_from_le_bytes(Bytes bytes);
Bytes u16_to_le_bytes(u16 value, Arena *arena);

u16 u16_from_ne_bytes(Bytes bytes);
Bytes u16_to_ne_bytes(u16 value, Arena *arena);
/* u16 */

/* i16 */
i16 i16_reverse_bits(i16 value);
i16 i16_swap_bytes(i16 value);
usize i16_leading_ones(i16 value);
usize i16_trailing_ones(i16 value);
usize i16_leading_zeros(i16 value);
usize i16_trailing_zeros(i16 value);
usize i16_count_zeros(i16 value);
usize i16_count_ones(i16 value);

i16 i16_to_be(i16 value);
i16 i16_from_be(i16 value);
i16 i16_from_be_bytes(Bytes bytes);
Bytes i16_to_be_bytes(i16 value, Arena *arena);

i16 i16_to_le(i16 value);
i16 i16_from_le(i16 value);
i16 i16_from_le_bytes(Bytes bytes);
Bytes i16_to_le_bytes(i16 value, Arena *arena);

i16 i16_from_ne_bytes(Bytes bytes);
Bytes i16_to_ne_bytes(i16 value, Arena *arena);
/* i16 */

/* u32 */
u32 u32_reverse_bits(u32 value);
u32 u32_swap_bytes(u32 value);
usize u32_leading_ones(u32 value);
usize u32_trailing_ones(u32 value);
usize u32_leading_zeros(u32 value);
usize u32_trailing_zeros(u32 value);
usize u32_count_zeros(u32 value);
usize u32_count_ones(u32 value);

u32 u32_to_be(u32 value);
u32 u32_from_be(u32 value);
u32 u32_from_be_bytes(Bytes bytes);
Bytes u32_to_be_bytes(u32 value, Arena *arena);

u32 u32_to_le(u32 value);
u32 u32_from_le(u32 value);
u32 u32_from_le_bytes(Bytes bytes);
Bytes u32_to_le_bytes(u32 value, Arena *arena);

u32 u32_from_ne_bytes(Bytes bytes);
Bytes u32_to_ne_bytes(u32 value, Arena *arena);
/* u32 */

#endif /* !__CLIB_INTEGERS_H__ */
