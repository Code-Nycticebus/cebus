/* counts the all the occurunces of words of a specified text file. */

#include "cebus.h"

// predicate function
static bool predicate(char c) { return c_is_space(c) || c_is_punct(c); }

typedef struct {
  Str word;
  i32 count;
} WordCount;

// sorting function
static CmpOrdering sort_by_occurence(const void *a_ptr, const void *b_ptr) {
  const WordCount *a = a_ptr;
  const WordCount *b = b_ptr;
  return b->count - a->count;
}

int main(int argc, const char **argv) {
  // Takes an argument
  Str file = argc < 2 ? STR(__FILE__) : str_from_cstr(argv[1]);

  // Initialize arena for allocation
  Arena arena = {0};

  // Create error
  Error error = ErrNew;
  // Read the entire file
  Str content = fs_file_read_str(file, &arena, &error);
  // Handle errors
  error_context(&error, {
    // catch a FILE_NOT_FOUND error
    if (error_code(FileError) == FS_NOT_FOUND) {
      // log error message
      cebus_log_error(STR_FMT, STR_ARG(error_msg()));
      return -1;
    }
  });

  // initialize the dynamic array
  DA(WordCount) words = {0};
  da_init(&words, &arena);

  // intialize the HashMap
  HashMap *word_idx = hm_create(&arena);

  usize total_words = 0;
  // Iterate over the content word by word
  for (Str word = {0}; str_try_chop_by_predicate(&content, predicate, &word);) {
    total_words++;

    // calculate hash of the current word
    u64 hash = str_hash(word);

    // search for the word inside of the HashMap
    const usize *idx = hm_get_usize(word_idx, hash);
    if (idx == NULL) {
      // Insert index into HashMap
      hm_insert_usize(word_idx, hash, words.len);
      // Push it into the dynamic array
      da_push(&words, (WordCount){.word = word, .count = 1});
    } else {
      // increase the count
      da_get(&words, *idx).count++;
    }
  }

  // Sort the array
  da_sort(&words, &words, sort_by_occurence);

  // print stats
  cebus_log_info("File: '" STR_FMT "'", STR_ARG(file));
  cebus_log_info("total words:  %" USIZE_FMT, total_words);
  cebus_log_info("unique words: %" USIZE_FMT, words.len);
  // print out the 3 most occuring words
  for (usize i = 0; i < 3; ++i) {
    cebus_log_info(" %" USIZE_FMT ": %d times: '" STR_FMT "'", i + 1, da_get(&words, i).count,
                   STR_ARG(da_get(&words, i).word));
  }

  // free the memory
  arena_free(&arena);
}
