#ifndef __CEBUS_BOOL_H__
#define __CEBUS_BOOL_H__

#include "cebus/core/defines.h"

bool bool_toggle(bool b);

bool bool_any(usize count, const bool *array);
bool bool_all(usize count, const bool *array);

#endif
