#include "cebus/core/error.h"

#include "cebus/core/debug.h"
#include "cebus/type/string.h"

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
  cebus_assert(e1.failure == false, "Init failed");
  cebus_assert(e1.panic_on_emit == false, "Init failed");
  cebus_assert(e1.location.file, "This should be set");
  cebus_assert(e1.location.line, "This should be set");
  cebus_assert(e1.location.function, "This should be set");
  cebus_assert(e1.arena.begin == NULL, "This should be set to NULL");

  Error *e2 = ErrPanic;
  cebus_assert(e2->failure == false, "Init failed");
  cebus_assert(e2->panic_on_emit == true, "Init failed");

  Error *e3 = ErrDefault;
  cebus_assert(e3 == ErrDefault, "Init failed");
  cebus_assert(e3 == NULL, "Init failed");
}

static void test_error_function(void) {
  Error err = ErrNew;
  u64 i = fn_that_fails(true, &err);
  cebus_assert(err.failure == true, "did not set err.failure correctly");
  cebus_assert(err.info->code == 69, "did not set err.info->code correctly");
  cebus_assert(i == 0, "Did not return correctly");

  error_context(&err, { error_except(); });
}

static void test_error_note_adding(void) {
  Error err = ErrNew;
  fn_that_fails_and_adds_note(true, &err);
  cebus_assert(err.info->code == 420, "did not set err2.error correctly");

  error_context(&err, { error_except(); });
}

static void test_error_except(void) {
  Error err = ErrNew;
  fn_that_fails_and_adds_note(true, &err);

  error_context(&err, { error_except(); });

  cebus_assert(err.failure == false, "Did not ignore correctly");
  cebus_assert(err.info == NULL, "Did not ignore correctly");
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
  error_propagate(&err, {});
  cebus_assert(err.info->locations.len == 3, "Propagate did not add any more locations!");

  error_context(&err, { error_except(); });
}

static void test_error_message(void) {
  Error error = ErrNew;
  error_emit(&error, -1, "MESSAGE");

  error_context(&error, {
    error_add_note("Note here!");
    cebus_assert(str_eq(error_msg(), STR("MESSAGE")), "Error message is wrong");
    error_set_msg("What");
    cebus_assert(str_eq(error_msg(), STR("What")), "Error message is wrong");
    error_except();
  });
}

int main(void) {
  test_error_creation();
  test_error_function();
  test_error_note_adding();
  test_error_except();
  test_error_match();
  test_error_propagate();
  test_error_message();
}
