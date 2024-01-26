#ifndef __CLIB_FLOATS_H__
#define __CLIB_FLOATS_H__

#include "core/defines.h"

#define FLOAT_DECL(T)                                                          \
  CONST bool T##_eq(T a, T b);                                                 \
  CONST bool T##_isnan(T value);                                               \
  CONST T T##_abs(T value);                                                    \
  CONST T T##_min(T a, T b);                                                   \
  CONST T T##_max(T a, T b);                                                   \
  CONST T T##_clamp(T min, T max, T value);                                    \
  CONST T T##_lerp(T min, T max, T value);                                     \
  CONST T T##_rad(T deg);                                                      \
  CONST T T##_deg(T rad);

FLOAT_DECL(f32)
FLOAT_DECL(f64)

#endif /* !__CLIB_FLOATS_H__ */
