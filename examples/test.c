#include "collection/da.h"
#include "collection/hashmap.h"
#include "core/arena.h"
#include "core/logging.h"
#include "os/io.h"
#include "type/string.h"

Arena arena = {0};

DA(Str) values = {0};
HashMap symbol_value;

typedef enum {
  INT,
  STR,
} Type;
HashMap types_hm;

HashMap symbol_types;

static void command(Str i) {
  i = str_trim(i);
  if (str_includes(i, '=')) {
    Str type = {0};
    if (str_try_chop_by_delim(&i, ' ', &type) == false) {
      clib_log_fatal("Type was not given!");
      return;
    }
    type = str_trim(type);
    HashValue *hv = hm_get(&types_hm, str_hash(type));
    if (hv == NULL) {
      clib_log_fatal("unkown type: \"" STR_FMT "\"", STR_ARG(type));
      return;
    }

    Str symbol = str_trim(str_chop_by_delim(&i, '='));
    if (symbol.len == 0) {
      clib_log_fatal("symbol was not given!");
      return;
    }
    Str value = str_trim(i);
    if (value.len == 0) {
      clib_log_fatal("value was not given!");
      return;
    }

    u64 hash = str_hash(symbol);
    hm_insert(&symbol_types, hash, *hv);
    da_push(&values, str_copy(str_trim(value), &arena));
    hm_insert(&symbol_value, hash, HashValue(ptr, &da_last(&values)));
  } else {
    u64 hash = str_hash(i);
    HashValue *value = hm_get(&symbol_value, hash);
    if (value) {
      Type type = (Type)hm_get(&symbol_types, hash)->as.i32;
      switch (type) {
      case INT: {
        clib_log_info("%" U64_FMT, str_to_u64(*(Str *)value->as.ptr));
      } break;
      case STR: {
        clib_log_info("\"" STR_FMT "\"", STR_ARG(*(Str *)value->as.ptr));
      } break;
      }
    } else {
      clib_log_info("Symbol not found: \"" STR_FMT "\"", STR_ARG(i));
    }
  }
}

int main(void) {
  da_init(&values, &arena);
  symbol_value = hm_create(&arena);

  symbol_types = hm_create(&arena);

  types_hm = hm_create(&arena);
  hm_insert(&types_hm, str_hash(STR("int")), HashValue(i32, INT));
  hm_insert(&types_hm, str_hash(STR("str")), HashValue(i32, STR));

  while (true) {
    Str i = input(STR("> "));
    if (str_eq(i, STR("q")) || str_eq(i, STR("quit"))) {
      break;
    } else {
      command(i);
    }
  }

  arena_free(&arena);
}
