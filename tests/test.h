#ifndef __TEST_H__
#define __TEST_H__

#include "core/arena.h" // IWYU pragma: export

#define TEST(name) static void name##_test(unused Arena *arena)

#define TEST_RUN(name, arena)                                                  \
  name##_test((arena));                                                        \
  arena_reset((arena))

#endif /* !__TEST_H__ */
