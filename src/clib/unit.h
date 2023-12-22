#ifndef __CLIB_UNIT_H__
#define __CLIB_UNIT_H__

#include "defines.h"

typedef bool (*test_fn)(void);

#define TEST(name) bool name(void)

#define TEST_SUITE(...)                                                        \
  int main(void) {                                                             \
    test_fn tests[] = {__VA_ARGS__};                                           \
    size_t n = sizeof(tests) / sizeof(tests[0]);                               \
    for (size_t i = 0; i < n; i++) {                                           \
      if (tests[i]()) {                                                        \
      }                                                                        \
    }                                                                          \
  }

#endif /* !__CLIB_UNIT_H__ */
