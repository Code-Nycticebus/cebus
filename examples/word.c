#include "clib/clib.h"

static bool predicate(char c) { return c_is_space(c) || c_is_punct(c); }

static CmpOrdering sort_by_occurence(const void *context, const void *a,
                                     const void *b) {
  return u32_compare_gt(*hm_get_u32(context, str_hash(*(Str *)a)),
                        *hm_get_u32(context, str_hash(*(Str *)b)));
}

int main(void) {
  Arena arena = {0};
  Str content = file_read_str(STR(__FILE__), &arena, ErrDefault);

  DA(Str) words = {0};
  da_init(&words, &arena);

  HashMap *occurences = hm_create(&arena);

  for (Str word = {0}; str_try_chop_by_predicate(&content, predicate, &word);) {
    u64 hash = str_hash(word);
    u32 *count = hm_get_u32_mut(occurences, hash);
    if (count == NULL) {
      da_push(&words, word);
      hm_insert_u32(occurences, hash, 1);
    } else {
      *count += 1;
    }
  }

  da_sort_ctx(&words, &words, sort_by_occurence, occurences);

  for (usize i = 0; i < 4; ++i) {
    clib_log_info("%" USIZE_FMT ": %d, " STR_FMT, i + 1,
                  *hm_get_u32(occurences, str_hash(words.items[i])),
                  STR_ARG(words.items[i]));
  }

  arena_free(&arena);
}
