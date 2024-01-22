#include "core/error.h"

#include "core/asserts.h"

static u64 fn_that_fails(bool fail, Error *error) {
  if (fail) {
    error_emit(error, 69, "function that fails failed o_o");
    return 0;
  }
  return 4;
}

static u64 fn_that_fails_and_adds_note(bool fail, Error *error) {
  u64 res = fn_that_fails(fail, error);
  error_context(error, {
    error_add_note("Note: function failed!");
    error_set_code(420);
  });

  return res;
}

int main(void) {
  Error err = ErrNew;
  u64 i = fn_that_fails(true, &err);
  clib_assert(err.failure == true, "did not set err.failure correctly");
  clib_assert(err.code == 69, "did not set err.error correctly");
  clib_assert(i == 0, "Did not return correctly");

  Error err2 = ErrNew;
  fn_that_fails_and_adds_note(true, &err2);
  clib_assert(err2.code == 420, "did not set err2.error correctly");

  error_context(&err2, {
    error_match({
      case 69:
        error_panic();

      case 420: {
        error_ignore();
      } break;
    });
  });

  clib_assert(err2.failure == false, "Did not ignore correctly");
  clib_assert(err2.msg_size == 0, "Did not ignore correctly");
}
