#ifndef __CLIB_FLOATS_H__
#define __CLIB_FLOATS_H__

#include "core/defines.h"

#define FLOAT_DECL(T)                                                          \
  bool CONST T##_eq(T a, T b);                                                 \
  bool CONST T##_isnan(T value);                                               \
  T CONST T##_abs(T value);                                                    \
  T CONST T##_min(T a, T b);                                                   \
  T CONST T##_max(T a, T b);                                                   \
  T CONST T##_clamp(T min, T max, T value);                                    \
  T CONST T##_lerp(T min, T max, T value);                                     \
  T CONST T##_rad(T deg);                                                      \
  T CONST T##_deg(T rad);

FLOAT_DECL(f32)
FLOAT_DECL(f64)

#endif /* !__CLIB_FLOATS_H__ */
