#include "clib/core/error.h"

#include "clib/core/assert.h"

#include <stdio.h>
#include <stdlib.h>

static u64 fn_that_fails(bool fail, Error *error) {
  if (fail) {
    error_emit(error, 69, "function that fails failed o_o");
    return 0;
  }
  return 4;
}

static u64 fn_that_fails_and_adds_note(bool fail, Error *error) {
  u64 res = fn_that_fails(fail, error);
  error_propagate(error, {
    error_add_note("Note: function failed!");
    error_set_code(420);
  });
  return res;
}

static void test_error_creation(void) {
  Error e1 = ErrNew;
  clib_assert(e1.failure == false, "Init failed");
  clib_assert(e1.panic_on_emit == false, "Init failed");
  clib_assert(e1.info.location.file, "This should be set");
  clib_assert(e1.info.location.line, "This should be set");
  clib_assert(e1.info.arena.begin == NULL, "This should be set to NULL");

  Error *e2 = ErrPanic;
  clib_assert(e2->failure == false, "Init failed");
  clib_assert(e2->panic_on_emit == true, "Init failed");

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
    switch (error_code(i32)) {
    case 420:
      error_except();
    }
  });
}

static void test_error_propagate(void) {
  Error err = ErrNew;
  fn_that_fails_and_adds_note(true, &err);
  error_propagate(&err, {
    break; // Jump out of context!
  });
  clib_assert(err.info.locations.len == 3,
              "Propagate did not add any more locations!");

  // Else there is a memory leak
  _error_internal_except(&err);
}

int main(void) {
  test_error_creation();
  test_error_function();
  test_error_note_adding();
  test_error_except();
  test_error_match();
  test_error_propagate();
}
