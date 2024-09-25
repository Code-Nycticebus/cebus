#include "bool.h"

bool bool_toggle(bool b) { return !b; }

bool bool_any(usize count, const bool *array) {
  for (usize i = 0; i < count; ++i) {
    if (array[i]) {
      return true;
    }
  }
  return false;
}

bool bool_all(usize count, const bool *array) {
  for (usize i = 0; i < count; ++i) {
    if (!array[i]) {
      return false;
    }
  }
  return true;
}
