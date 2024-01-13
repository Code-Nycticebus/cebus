#ifndef __CLIB_FLOATS_H__
#define __CLIB_FLOATS_H__

#include "core/defines.h"

bool f32_eq(f32 a, f32 b);
bool f32_isnan(f32 value);

f32 f32_abs(f32 value);

f32 f32_min(f32 a, f32 b);
f32 f32_max(f32 a, f32 b);
f32 f32_clamp(f32 min, f32 max, f32 value);
f32 f32_lerp(f32 min, f32 max, f32 value);

f32 f32_rad(f32 deg);

#endif /* ifndef __CLIB_FLOATS_H__ */
