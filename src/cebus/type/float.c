#include "float.h" // IWYU pragma: keep

#define FLOAT_IMPL(T, BITS)                                                                        \
  bool T##_eq(T a, T b) { return T##_abs(a - b) < F##BITS##_EPSILON; }                             \
  bool T##_eq_eps(T a, T b, T epsilon) { return T##_abs(a - b) < epsilon; }                        \
  bool T##_isnan(T value) { return value != value; }                                               \
  T T##_abs(T value) {                                                                             \
    u##BITS *i = (u##BITS *)&value;                                                                \
    *i &= ~((u##BITS)0x8 << (BITS - 1 - 3));                                                       \
    return value;                                                                                  \
  }                                                                                                \
  T T##_min(T a, T b) { return a < b ? a : b; }                                                    \
  T T##_max(T a, T b) { return a > b ? a : b; }                                                    \
  T T##_clamp(T min, T max, T value) { return value < min ? min : value > max ? max : value; }     \
  T T##_lerp(T min, T max, T value) { return min + value * (max - min); }                          \
  T T##_rad(T deg) { return deg * ((T)F64_PI / 180.0f); }                                          \
  T T##_deg(T rad) { return rad * ((T)F64_PI / 180.0f); }

FLOAT_IMPL(f32, 32)
FLOAT_IMPL(f64, 64)

#undef FLOAT_IMPL
