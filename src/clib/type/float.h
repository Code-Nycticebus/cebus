/* DOCUMENTATION
## Functions

These functions are available for `f32` and `f64`.

- `f32_eq(a, b)`: Tests if two `f32` are equal.
- `f32_eq_eps(a, b)`: Tests if two `f32` are equal within a give epsilon.
- `f32_isnan(value)`: Tests if `f32` is `nan`.
- `f32_abs(value)`: Returns the absolute value.
- `f32_min(a, b)`: Returns the min of `a` or `b`.
- `f32_max(a, b)` Returns the max of `a` or b.
- `f32_clamp(min, max, value)`: Clamps a `f32` between `min` and `max`.
- `f32_lerp(min, max, value)`: Linear interpolation between `min` and `max`.
- `f32_rad(deg)`: Converts degrees to radians.
- `f32_deg(rad)`: Converts radians to degrees.
*/

#ifndef __CLIB_FLOATS_H__
#define __CLIB_FLOATS_H__

#include "clib/core/defines.h"

#define FLOAT_DECL(T)                                                          \
  CONST_FN bool T##_eq(T a, T b);                                              \
  CONST_FN bool T##_eq_eps(T a, T b, T epsilon);                               \
  CONST_FN bool T##_isnan(T value);                                            \
  CONST_FN T T##_abs(T value);                                                 \
  CONST_FN T T##_min(T a, T b);                                                \
  CONST_FN T T##_max(T a, T b);                                                \
  CONST_FN T T##_clamp(T min, T max, T value);                                 \
  CONST_FN T T##_lerp(T min, T max, T value);                                  \
  CONST_FN T T##_rad(T deg);                                                   \
  CONST_FN T T##_deg(T rad);

FLOAT_DECL(f32)
FLOAT_DECL(f64)

#undef FLOAT_DECL

#endif /* !__CLIB_FLOATS_H__ */
