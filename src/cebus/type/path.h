#ifndef __CEBUS_PATH_H__
#define __CEBUS_PATH_H__

#include "cebus/collection/da.h"
#include "cebus/core/arena.h"
#include "cebus/core/defines.h" // IWYU pragma: export

typedef DA(Path) PathDa;

Path _path_new(Arena *arena, ...);
#define path_new(arena, ...) _path_new(arena, __VA_ARGS__, (Str){0})

Path path_join(Arena *arena, PathDa *da);

Str path_name(Path path);
Str path_parent(Path path);

#endif /* !__CEBUS_PATH_H__ */
