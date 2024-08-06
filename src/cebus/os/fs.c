#include "fs.h"

#include "cebus/core/error.h"
#include "cebus/type/string.h"
#include "cebus/type/utf8.h"
#include "io.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

static usize file_size(FILE *handle, Error *error) {
  fseek(handle, 0, SEEK_END);
  const long size = ftell(handle);
  fseek(handle, 0, SEEK_SET);
  if (size < 0) {
    error_emit(error, errno, "Could not get file size: %s", strerror(errno));
    return 0;
  }
  return (usize)size;
}

////////////////////////////////////////////////////////////////////////////

FILE *fs_file_open(Str filename, const char *mode, Error *error) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);
  errno = 0;
  FILE *handle = fopen(_filename, mode);
  if (handle == NULL) {
    error_emit(error, errno, "Could not open file: '%s': %s", _filename,
               strerror(errno));
  }
  return handle;
}

void fs_file_close(FILE *file, Error *error) {
  if (file == NULL) {
    error_emit(error, FS_INVALID, "can't close a FILE* that is NULL");
    return;
  }
  errno = 0;
  int ret = fclose(file);
  if (ret == EOF) {
    error_emit(error, errno, "closing file failed: %s", strerror(errno));
  }
}

Bytes fs_file_read_bytes(Str filename, Arena *arena, Error *error) {
  Bytes result = {0};
  FILE *handle = fs_file_open(filename, "r", error);
  error_propagate(error, { goto defer; });
  usize size = file_size(handle, error);
  error_propagate(error, { goto defer; });

  u8 *buffer = arena_alloc(arena, size);
  result = io_read_bytes(handle, size, buffer, error);
  error_propagate(error, { goto defer; });

defer:
  if (handle) {
    fs_file_close(handle, error);
  }
  return result;
}

Str fs_file_read_str(Str filename, Arena *arena, Error *error) {
  Bytes bytes = fs_file_read_bytes(filename, arena, error);
  error_propagate(error, { return (Str){0}; });
  return str_from_bytes(bytes);
}

Utf8 fs_file_read_utf8(Str filename, Arena *arena, Error *error) {
  Utf8 res = {0};
  Bytes bytes = fs_file_read_bytes(filename, arena, error);
  error_propagate(error, { return (Utf8){0}; });
  res = utf8_decode(bytes, error);
  error_propagate(error, { return (Utf8){0}; });
  return res;
}

void fs_file_write_bytes(Str filename, Bytes bytes, Error *error) {
  FILE *handle = fs_file_open(filename, "w", error);
  error_propagate(error, { goto defer; });

  io_write_bytes(handle, bytes, error);
  error_propagate(error, { goto defer; });

defer:
  if (handle) {
    fclose(handle);
  }
}

void fs_file_write_str(Str filename, Str content, Error *error) {
  fs_file_write_bytes(filename, str_to_bytes(content), error);
}

void fs_file_write_utf8(Str filename, Utf8 content, Error *error) {
  Bytes bytes = utf8_encode(content, error);
  error_propagate(error, { return; });
  fs_file_write_bytes(filename, bytes, error);
}

void fs_rename(Str old_path, Str new_path, Error *error) {
  char _old_path[FILENAME_MAX] = {0};
  memcpy(_old_path, old_path.data, old_path.len);

  char _new_path[FILENAME_MAX] = {0};
  memcpy(_new_path, new_path.data, new_path.len);

  errno = 0;
  int ret = rename(_old_path, _new_path);
  if (ret == -1) {
    error_emit(error, errno, "Could not rename the file: " STR_FMT ": %s",
               STR_ARG(old_path), strerror(errno));
  }
}

void fs_remove(Str path, Error *error) {
  char _path[FILENAME_MAX] = {0};
  memcpy(_path, path.data, path.len);

  errno = 0;
  int ret = remove(_path);
  if (ret == -1) {
    error_emit(error, errno, "Could not remove the file: " STR_FMT ": %s",
               STR_ARG(path), strerror(errno));
  }
}

////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)

#define _BSD_SOURCE
#include <dirent.h>
#include <sys/stat.h> // For struct stat and S_ISDIR
#include <unistd.h>

typedef struct Node {
  Str dir;
  DIR *handle;
  struct Node *next;
} Node;

bool fs_exists(Str path) {
  char _path[FILENAME_MAX] = {0};
  memcpy(_path, path.data, path.len);
  return access(_path, 0) == 0;
}

bool fs_is_dir(Str path) {
  char _path[FILENAME_MAX] = {0};
  memcpy(_path, path.data, path.len);

  // Get entry information
  struct stat entry_info;
  if (stat(_path, &entry_info) == -1) {
    return false;
  }

  return S_ISDIR(entry_info.st_mode);
}

FsIterator fs_iter_begin(Str dir, bool recursive, Arena *arena, Error *error) {
  FsIterator it = {.arena = arena, .recursive = recursive, .error = error};

  Node *node = arena_calloc_chunk(&it.scratch, sizeof(Node));
  node->dir = str_copy(dir, &it.scratch);
  // Open the directory
  node->handle = opendir(node->dir.data);
  if (node->handle == NULL) {
    error_emit(error, errno, "opendir failed: %s", strerror(errno));
    return it;
  }

  it.stack = node;
  fs_iter_next(&it);
  return it;
}

void fs_iter_next(FsIterator *it) {
  while (it->stack != NULL) {
    Node *current = it->stack;

    struct dirent *entry = readdir(current->handle);
    if (entry == NULL) {
      closedir(current->handle);
      it->stack = current->next;
      arena_free_chunk(&it->scratch, current);
      continue;
    }

    // skip "." and ".." directories
    if (strncmp(entry->d_name, ".", 1) == 0 ||
        strncmp(entry->d_name, "..", 2) == 0) {
      continue;
    }

    Str fullpath = str_format(it->arena, STR_FMT "/%s", STR_ARG(current->dir),
                              entry->d_name);

    // Get entry information
    struct stat entry_info;
    if (stat(fullpath.data, &entry_info) == -1) {
      error_emit(it->error, errno, "stat failed: %s", strerror(errno));
      return;
    }

    it->current.path = fullpath;
    it->current.directory = S_ISDIR(entry_info.st_mode);

    if (it->current.directory && it->recursive) {
      printf("DIR: " STR_FMT "\n", STR_ARG(it->current.path));
      Node *node = arena_calloc_chunk(&it->scratch, sizeof(Node));
      node->handle = opendir(fullpath.data);
      if (node->handle == NULL) {
        error_emit(it->error, errno, "opendir failed: %s", strerror(errno));
        return;
      }
      node->dir = fullpath;
      node->next = it->stack;
      it->stack = node;
    }

    return;
  }
}

bool fs_iter_end(FsIterator *it) {
  error_propagate(it->error, {
    arena_free(&it->scratch);
    return false;
  });
  if (it->stack == NULL) {
    arena_free(&it->scratch);
    return false;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include <io.h>

bool fs_exists(Str filename) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);
  return _access(_filename, 0) == 0;
}

#error "fs_is_dir not implemented"
#error "fs_iter_begin not implemented"
#error "fs_iter_next not implemented"
#error "fs_iter_end not implemented"

//////////////////////////////////////////////////////////////////////////////
#endif
