#include "collection/da.h"
#include "collection/hashmap.h"
#include "core/arena.h"
#include "core/logging.h"
#include "os/io.h"
#include "type/string.h"

int main(void) {
  Arena arena = {0};
  DA(Str) values = {0};
  da_init(&values, &arena);
  HashMap symbols = hm_create(&arena);

  while (true) {
    Str i = input(STR("> "));
    if (str_eq(i, STR("q")) || str_eq(i, STR("quit"))) {
      break;
    } else if (str_includes(i, '=')) {
      Str symbol = str_trim(str_chop_by_delim(&i, '='));
      Str value = str_copy(str_trim(i), values.arena);
      da_push(&values, value);
      hm_insert(&symbols, str_hash(symbol), HashValue(ptr, &da_last(&values)));
    } else {
      HashValue *value = hm_get(&symbols, str_hash(i));
      if (value) {
        clib_log_info(STR_FMT, STR_ARG(*(Str *)value->as.ptr));
      } else {
        clib_log_info("Symbol not found: " STR_FMT, STR_ARG(i));
      }
    }
  }

  arena_free(&arena);
}
