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
Str path_suffix(Path path);
Str path_stem(Path path);
Path path_parent(Path path);

// TODO
// keep it io-less. i want a "pure" path api
// bool path_relative(Path path);
// bool path_absolute(Path path);
// bool path_relative_to(Path p1, Path p2);
// Path path_resolve(Path path);
// void path_collect_parts(Path path, PathDa* da);
// void path_collect_parents(Path path, PathDa* da);

#endif /* !__CEBUS_PATH_H__ */
