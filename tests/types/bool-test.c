#include "cebus/type/bool.h"

#include "cebus/collection/da.h"
#include "cebus/core/arena.h"
#include "cebus/core/debug.h"

static void bool_test_any(void) {
  Arena arena = {0};
  DA(bool) array = da_new(&arena);
  da_init_static(&array, &arena,
                 (bool[]){
                     true,
                     false,
                     true,
                 });

  cebus_assert(bool_any(DA_ARG(&array)), "");
  cebus_assert(!bool_all(DA_ARG(&array)), "");

  arena_free(&arena);
}

static void bool_test_all(void) {
  Arena arena = {0};
  DA(bool) array = da_new(&arena);
  da_init_static(&array, &arena,
                 (bool[]){
                     true,
                     true,
                     true,
                 });

  cebus_assert(bool_any(DA_ARG(&array)), "");
  cebus_assert(bool_all(DA_ARG(&array)), "");

  arena_free(&arena);
}

static void bool_test_none(void) {
  Arena arena = {0};
  DA(bool) array = da_new(&arena);
  da_init_static(&array, &arena,
                 (bool[]){
                     false,
                     false,
                     false,
                 });

  cebus_assert(!bool_any(DA_ARG(&array)), "");
  cebus_assert(!bool_all(DA_ARG(&array)), "");

  arena_free(&arena);
}

static void bool_test_toggle(void) {
  cebus_assert(bool_toggle(true) == false, "");
  cebus_assert(bool_toggle(false) == true, "");
}

int main(void) {
  bool_test_any();
  bool_test_all();
  bool_test_none();
  bool_test_toggle();
}
