#include "core/error.h"

#include "core/asserts.h"

#include <string.h>

u64 fn_that_fails(bool fail, Error *error);

u64 fn_that_fails(bool fail, Error *error) {
  if (fail) {
    Err(error, 69, "fn_that_fails(): function that fails failed °-°. '%s'",
        fail ? "true" : "false");
    return 0;
  }
  return 4;
}

static u64 fn_that_fails_and_adds_note(bool fail, Error *error) {
  fn_that_fails(fail, error);
  if (ErrFail(error)) {
    ErrNote(error, "Note: how did the function that fails, fail! '%s'",
            fail ? "true" : "false");
    return 0;
  }
  return 4;
}

int main(void) {
  Error err = ErrNew;
  u64 i = fn_that_fails(true, &err);
  clib_assert(err.failure == true, "did not set err.failure correctly");
  clib_assert(err.error == 69, "did not set err.error correctly");
  clib_assert(i == 0, "Did not return correctly");

  Error err2 = ErrNew;
  fn_that_fails_and_adds_note(true, &err2);
  clib_assert(
      strcmp(err2.message,
             "/home/nycticebus/Code/c/clib/tests/core/error-test.c:12:\n"
             "fn_that_fails(): function that fails failed °-°. 'true'\n"
             "/home/nycticebus/Code/c/clib/tests/core/error-test.c:22:\n"
             "Note: how did the function that fails, fail! 'true'") == 0,
      "Message is not correct");
  clib_assert(err2.error == 69, "did not set err2.error correctly");
}
