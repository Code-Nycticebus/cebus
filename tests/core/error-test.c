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

static void test_error_creation(void) {
  Error e1 = ErrNew;
  clib_assert(e1.failure == false, "Init failed");
  clib_assert(e1.panic_instantly == false, "Init failed");
  clib_assert(e1.info.file, "This should be set");
  clib_assert(e1.info.line, "This should be set");

  Error *e2 = ErrPanic;
  clib_assert(e2->failure == false, "Init failed");
  clib_assert(e2->panic_instantly == true, "Init failed");

  Error *e3 = ErrDefault;
  clib_assert(e3 == ErrDefault, "Init failed");
  clib_assert(e3 == NULL, "Init failed");
}

static void test_error_function(void) {
  Error err = ErrNew;
  u64 i = fn_that_fails(true, &err);
  clib_assert(err.failure == true, "did not set err.failure correctly");
  clib_assert(err.info.code == 69, "did not set err.error correctly");
  clib_assert(i == 0, "Did not return correctly");
}

static void test_error_note_adding(void) {
  Error err = ErrNew;
  fn_that_fails_and_adds_note(true, &err);
  clib_assert(err.info.code == 420, "did not set err2.error correctly");
}

static void test_error_except(void) {
  Error err = ErrNew;
  fn_that_fails_and_adds_note(true, &err);

  error_context(&err, { error_except(); });

  clib_assert(err.failure == false, "Did not ignore correctly");
  clib_assert(err.info.msg.len == 0, "Did not ignore correctly");
}

static void test_error_match(void) {
  Error err = ErrNew;
  fn_that_fails_and_adds_note(true, &err);

  error_context(&err, {
    error_match({
      case 69:
        clib_assert(false, "Should not execute this");
      case 420: {
        error_except();
      } break;
    });
  });
}

int main(void) {
  test_error_creation();
  test_error_function();
  test_error_note_adding();
  test_error_except();
  test_error_match();
}
