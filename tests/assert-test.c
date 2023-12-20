#include "clib/asserts.h"

int main(void) {
  clib_assert_warn(false, "");
  clib_assert(false, "");
}
