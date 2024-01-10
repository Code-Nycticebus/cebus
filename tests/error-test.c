#include "core/error.h"

#include "clib/asserts.h"

u64 fn_that_fails(bool fail, Error *error);

u64 fn_that_fails(bool fail, Error *error) {
  if (fail) {
    Err(error, 69, "fn_that_fails(): function that fails failed °-°. %p",
        (void *)error);
    return 0;
  }
  return 4;
}

int main(void) {
  Error err = {0};
  u64 i = fn_that_fails(true, &err);
  clib_assert(err.failure == true, "did not set err.failure correctly");
  clib_assert(err.error == 69, "did not set err.error correctly");
  clib_assert(i == 0, "Did not return correctly");
}
