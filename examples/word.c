/* counts the all the occurunces of words inside itself. */

#include "clib/clib.h"

// predicate function
static bool predicate(char c) { return c_is_space(c) || c_is_punct(c); }

// sorting function
static CmpOrdering sort_by_occurence(const void *context, const void *a,
                                     const void *b) {
  return u32_compare_gt(*hm_get_u32(context, str_hash(*(Str *)a)),
                        *hm_get_u32(context, str_hash(*(Str *)b)));
}

int main(void) {
  // Initialize arena for allocation
  Arena arena = {0};

  // Read the entire file
  Str content = file_read_str(STR(__FILE__), &arena, ErrDefault);

  // initialize the dynamic array
  DA(Str) words = {0};
  da_init(&words, &arena);

  // intialize the HashMap
  HashMap *occurences = hm_create(&arena);

  // Iterate over the content word by word
  for (Str word = {0}; str_try_chop_by_predicate(&content, predicate, &word);) {

    // calculate hash of the current word
    u64 hash = str_hash(word);

    // search for the word inside of the HashMap
    u32 *count = hm_get_u32_mut(occurences, hash);

    // If its not found
    if (count == NULL) {
      // Push it into the dynamic array
      da_push(&words, word);
      // Insert it into HashMap with a count of one
      hm_insert_u32(occurences, hash, 1);
    } else {
      // increase the count
      *count += 1;
    }
  }

  // Sort the array using the sorting function
  da_sort_ctx(&words, &words, sort_by_occurence, occurences);

  // print out the first 4 words
  for (usize i = 0; i < 4; ++i) {
    clib_log_info("%" USIZE_FMT ": %d, " STR_FMT, i + 1,
                  *hm_get_u32(occurences, str_hash(words.items[i])),
                  STR_ARG(words.items[i]));
  }

  // free the memory
  arena_free(&arena);
}
