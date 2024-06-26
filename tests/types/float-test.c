#include "cebus/type/float.h"

#include "cebus/core/debug.h"

static void test_f32_eq(void) {
  cebus_assert(f32_eq(3.14f, 3.14f) == true, "Floats should be eq"); // NOLINT
  cebus_assert(!f32_eq(.14f, .15f), "Floats should not be eq");      // NOLINT
}

static void test_f32_eq_eps(void) {
  cebus_assert(f32_eq_eps(3.143f, 3.14f, 1e-2f), "should be eq");   // NOLINT
  cebus_assert(!f32_eq_eps(.14f, .15f, 1e-2f), "should not be eq"); // NOLINT
}

static void test_f32_abs(void) {
  const f32 value = f32_abs(-3.14f);
  cebus_assert(f32_eq(value, 3.14f), // NOLINT
               "Absolute value was not calculated correctly");
}

static void test_f32_isnan(void) {
  const f32 n = F32_NAN;
  const f32 nn = 0.1f;
  cebus_assert(f32_isnan(n) == true, "n should be nan");
  cebus_assert(f32_isnan(nn) == false, "nn should not be nan");
}

static void test_f32_math(void) {
  const f32 min = f32_min(3.1f, 4.2f);
  cebus_assert(f32_eq(min, 3.1f), "min was not correct"); // NOLINT

  const f32 max = f32_max(3.1f, 4.2f);
  cebus_assert(f32_eq(max, 4.2f), "max was not correct"); // NOLINT

  const f32 clamp = f32_clamp(3.1f, 3.5f, 5.f);
  cebus_assert(f32_eq(clamp, 3.5f), "Did not clamp correctly"); // NOLINT

  const f32 lerp = f32_lerp(10.f, 20.f, .5f);
  cebus_assert(f32_eq(lerp, 15.f), "Did not lerp correctly"); // NOLINT

  const f32 rad = f32_rad(90.f);
  cebus_assert(f32_eq(rad, 1.570796371f), "Rad was not correct"); // NOLINT
}

static void test_f64_eq(void) {
  cebus_assert(f64_eq(3.14, 3.14) == true, "Floats should be eq");    // NOLINT
  cebus_assert(f64_eq(.14, .15) == false, "Floats should not be eq"); // NOLINT
}

static void test_f64_eq_eps(void) {
  cebus_assert(f64_eq_eps(3.143, 3.14, 1e-2), "should be eq");   // NOLINT
  cebus_assert(!f64_eq_eps(.14, .16, 1e-2), "should not be eq"); // NOLINT
}

static void test_f64_abs(void) {
  const f64 value = f64_abs(-3.14);
  cebus_assert(f64_eq(value, 3.14), // NOLINT
               "Absolute value was not calculated correctly");
}

static void test_f64_isnan(void) {
  const f64 n = F64_NAN;
  const f64 nn = 0.1;
  cebus_assert(f64_isnan(n) == true, "n should be nan");
  cebus_assert(f64_isnan(nn) == false, "nn should not be nan");
}

static void test_f64_math(void) {
  const f64 min = f64_min(3.1, 4.2);
  cebus_assert(f64_eq(min, 3.1), "min was not correct"); // NOLINT

  const f64 max = f64_max(3.1, 4.2);
  cebus_assert(f64_eq(max, 4.2), "max was not correct"); // NOLINT

  const f64 clamp = f64_clamp(3.1, 3.5, 5.);
  cebus_assert(f64_eq(clamp, 3.5),
               "Did not clamp// NOLINT correctly"); // NOLINT

  const f64 lerp = f64_lerp(10., 20., .5);
  cebus_assert(f64_eq(lerp, 15.), "Did not lerp correctly"); // NOLINT

  const f64 rad = f64_rad(90.);
  const f64 res = 1.57079632679489656;
  cebus_assert(f64_eq(rad, res), "Rad was not correct");
}

int main(void) {
  test_f32_eq();
  test_f32_eq_eps();
  test_f32_abs();
  test_f32_isnan();
  test_f32_math();

  test_f64_eq();
  test_f64_eq_eps();
  test_f64_abs();
  test_f64_isnan();
  test_f64_math();
}
