#include "clib/da.h"

#include <assert.h>
#include <stddef.h>

typedef struct {
  DA(size_t);
} ListSize;

void test_da(void) {
  const size_t n = 10;
  ListSize list = {0};
  da_init(&list, 2);
  for (size_t i = 0; i < n; ++i) {
    da_push(&list, i + 1);
  }

  for (size_t i = 0; i < list.len; ++i) {
    assert(list.items[i] == i + 1);
  }

  da_free(&list);
}

int main(void) { test_da(); }
