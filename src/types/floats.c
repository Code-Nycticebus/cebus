#include "floats.h"

// For bit manipulation
typedef union {
  f32 f;
  u32 i;
} f32_bits;

bool f32_eq(f32 a, f32 b) { return f32_abs(a - b) < F32_EPSILON; }

bool f32_isnan(f32 value) {
  f32_bits u = {.f = value};
  return ((u.i >> 23) & 0xFF) == 0xFF && (u.i & 0x7FFFFF) != 0; // NOLINT
}

f32 f32_abs(f32 value) {
  f32_bits u = {.f = value};
  u.i &= 0x7FFFFFFF; // NOLINT
  return u.f;
}

f32 f32_min(f32 a, f32 b) { return a < b ? a : b; }
f32 f32_max(f32 a, f32 b) { return a > b ? a : b; }
f32 f32_clamp(f32 min, f32 max, f32 value) {
  return value < min ? min : value > max ? max : value;
}
f32 f32_lerp(f32 min, f32 max, f32 value) { return min + value * (max - min); }

f32 f32_rad(f32 deg) { return deg * ((float)PI / 180.0f); } // NOLINT
