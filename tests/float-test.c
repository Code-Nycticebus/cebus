#include "types/floats.h"

#include "core/asserts.h"

static void test_f32_eq(void) {
  clib_assert(f32_eq(3.14f, 3.14f) == true, "Floats should be eq");    // NOLINT
  clib_assert(f32_eq(.14f, .15f) == false, "Floats should not be eq"); // NOLINT
}

static void test_f32_abs(void) {
  const f32 value = f32_abs(-3.14f);
  clib_assert(f32_eq(value, 3.14f), // NOLINT
              "Absolute value was not calculated correctly");
}

static void test_f32_isnan(void) {
  const f32 n = (0.0f / 0.0f);
  const f32 nn = 0.1f;
  clib_assert(f32_isnan(n) == true, "n should be nan");
  clib_assert(f32_isnan(nn) == false, "nn should be nan");
}

static void test_f32_math(void) {
  const f32 min = f32_min(3.1f, 4.2f);
  clib_assert(f32_eq(min, 3.1f), "min was not correct"); // NOLINT

  const f32 max = f32_max(3.1f, 4.2f);
  clib_assert(f32_eq(max, 4.2f), "max was not correct"); // NOLINT

  const f32 clamp = f32_clamp(3.1f, 3.5f, 5.f);
  clib_assert(f32_eq(clamp, 3.5f), "Did not clamp correctly"); // NOLINT

  const f32 lerp = f32_lerp(10.f, 20.f, .5f);
  clib_assert(f32_eq(lerp, 15.f), "Did not lerp correctly"); // NOLINT

  const f32 rad = f32_rad(90.f);
  clib_assert(f32_eq(rad, 1.570796371f), "Rad was not correct"); // NOLINT
}

int main(void) {
  test_f32_eq();
  test_f32_abs();
  test_f32_isnan();
  test_f32_math();
}
