#include "clib/unit.h"

TEST(my_test) { return false; }
TEST(my_test2) { return false; }

TEST_SUITE(my_test, my_test2)
