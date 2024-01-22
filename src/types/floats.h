#ifndef __CLIB_FLOATS_H__
#define __CLIB_FLOATS_H__

#include "core/defines.h"

#define FLOAT_DECL(T)                                                          \
  bool const_fn T##_eq(T a, T b);                                              \
  bool const_fn T##_isnan(T value);                                            \
  T const_fn T##_abs(T value);                                                 \
  T const_fn T##_min(T a, T b);                                                \
  T const_fn T##_max(T a, T b);                                                \
  T const_fn T##_clamp(T min, T max, T value);                                 \
  T const_fn T##_lerp(T min, T max, T value);                                  \
  T const_fn T##_rad(T deg);                                                   \
  T const_fn T##_deg(T rad);

FLOAT_DECL(f32)
FLOAT_DECL(f64)

#endif /* !__CLIB_FLOATS_H__ */
