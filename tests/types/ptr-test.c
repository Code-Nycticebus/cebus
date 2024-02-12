#include <stdbool.h>
#include <stdio.h>

#define PTR(T) T *
#define ptr_context(T, var, assign, ...)                                       \
  do {                                                                         \
    T *var = assign;                                                           \
    if (ptr_safe(var)) {                                                       \
      __VA_ARGS__                                                              \
    }                                                                          \
  } while (0)

static bool ptr_safe(void *ptr) { return ptr != NULL; }

static PTR(int) ret_ptr(void) {
  static int a = 420;
  (void)a;
  return &a;
}

int main(void) {
  ptr_context(int, a, ret_ptr(), { printf("ptr: %d\n", *a); });
}
