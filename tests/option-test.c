#include "clib/option.h"
#include <assert.h>
#include <stdio.h>

uint64_t add(uint64_t a) { return a + 4; }

Option ret_option(bool fail) {
  if (fail) {
    return option_none();
  }
  Option o = option_some_u64(0);
  return option_map_u64(o, add);
}

Option do_things(uint64_t val) {
  Option o = ret_option(false);
  option_propagate(o);
  uint64_t value = option_unwrap_u64(o);
  return option_some_u64(value + val);
}

void test_unwrap(void) {
  const uint64_t n = 420;
  Option o = do_things(n);
  uint64_t value = option_expect_u64(o, "Expected value");
  assert(value == n + 4);
}

int main(void) { test_unwrap(); }
