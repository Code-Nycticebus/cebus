#ifndef __TEST_H__
#define __TEST_H__

#include "clib/arena.h" // IWYU pragma: export
#include "clib/platform.h"

#if defined(CLANG) || defined(GCC)
#define UNUSED __attribute((unused))
#else
#define UNUSED
#endif

#define TEST(name) static void name##_test(UNUSED Arena *arena)

#define TEST_RUN(name, arena)                                                  \
  name##_test((arena));                                                        \
  arena_reset((arena))

#endif // !__TEST_H__
