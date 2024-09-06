#include "fs.h"

#include "cebus/core/debug.h"
#include "cebus/core/error.h"
#include "cebus/type/path.h"
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

FILE *fs_file_open(Path path, const char *mode, Error *error) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, path.data, path.len);
  errno = 0;
  FILE *handle = fopen(_filename, mode);
  if (handle == NULL) {
    error_emit(error, errno, "Could not open file: '%s': %s", _filename, strerror(errno));
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

Bytes fs_file_read_bytes(Path filename, Arena *arena, Error *error) {
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

Str fs_file_read_str(Path filename, Arena *arena, Error *error) {
  Bytes bytes = fs_file_read_bytes(filename, arena, error);
  error_propagate(error, { return (Str){0}; });
  return str_from_bytes(bytes);
}

Utf8 fs_file_read_utf8(Path filename, Arena *arena, Error *error) {
  Utf8 res = {0};
  Bytes bytes = fs_file_read_bytes(filename, arena, error);
  error_propagate(error, { return (Utf8){0}; });
  res = utf8_decode(bytes, error);
  error_propagate(error, { return (Utf8){0}; });
  return res;
}

void fs_file_write_bytes(Path filename, Bytes bytes, Error *error) {
  FILE *handle = fs_file_open(filename, "w", error);
  error_propagate(error, { goto defer; });

  io_write_bytes(handle, bytes, error);
  error_propagate(error, { goto defer; });

defer:
  if (handle) {
    fclose(handle);
  }
}

void fs_file_write_str(Path filename, Str content, Error *error) {
  fs_file_write_bytes(filename, str_to_bytes(content), error);
}

void fs_file_write_utf8(Path filename, Utf8 content, Error *error) {
  Bytes bytes = utf8_encode(content, error);
  error_propagate(error, { return; });
  fs_file_write_bytes(filename, bytes, error);
}

void fs_rename(Path old_path, Path new_path, Error *error) {
  char _old_path[FILENAME_MAX] = {0};
  memcpy(_old_path, old_path.data, old_path.len);

  char _new_path[FILENAME_MAX] = {0};
  memcpy(_new_path, new_path.data, new_path.len);

  errno = 0;
  int ret = rename(_old_path, _new_path);
  if (ret == -1) {
    error_emit(error, errno, "Could not rename the file: " STR_FMT ": %s", STR_ARG(old_path),
               strerror(errno));
  }
}

void fs_remove(Path path, Error *error) {
  char _path[FILENAME_MAX] = {0};
  memcpy(_path, path.data, path.len);

  errno = 0;
  int ret = remove(_path);
  if (ret == -1) {
    error_emit(error, errno, "Could not remove the file: " STR_FMT ": %s", STR_ARG(path),
               strerror(errno));
  }
}

bool fs_iter_next_filter(FsIter *it, bool (*filter)(FsEntity *entity)) {
  while (fs_iter_next(it)) {
    if (filter(&it->current)) {
      return true;
    }
  }
  return false;
}

bool fs_iter_next_directory(FsIter *it) {
  while (fs_iter_next(it)) {
    if (it->current.is_dir) {
      return true;
    }
  }
  return false;
}

bool fs_iter_next_files(FsIter *it) {
  while (fs_iter_next(it)) {
    if (!it->current.is_dir) {
      return true;
    }
  }
  return false;
}

bool fs_iter_next_suffix(FsIter *it, Str suffix) {
  while (fs_iter_next_files(it)) {
    if (str_eq(path_suffix(it->current.path), suffix)) {
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)

#include <dirent.h>
#include <sys/stat.h> // For struct stat and S_ISDIR
#include <unistd.h>

typedef struct FsNode {
  DIR *handle;
  struct FsNode *next;
  char name[];
} FsNode;

bool fs_exists(Path path) {
  char _path[FILENAME_MAX] = {0};
  memcpy(_path, path.data, path.len);
  return access(_path, 0) == 0;
}

bool fs_is_dir(Path path) {
  char _path[FILENAME_MAX] = {0};
  memcpy(_path, path.data, path.len);

  struct stat entry_info;
  if (stat(_path, &entry_info) == -1) {
    return false;
  }

  return S_ISDIR(entry_info.st_mode);
}

FsIter fs_iter_begin(Path directory, bool recursive) {
  FsIter it = {.recursive = recursive, .error = ErrNew};

  const usize size = sizeof(FsNode) + directory.len + 1;
  FsNode *node = arena_calloc_chunk(&it.scratch, size);
  memcpy(node->name, directory.data, directory.len);

  node->handle = opendir(node->name);
  if (node->handle == NULL) {
    error_emit(&it.error, errno, "opendir failed: %s", strerror(errno));
    return it;
  }
  it._stack = node;
  return it;
}

void fs_iter_end(FsIter *it, Error *error) {
  error_propagate(&it->error, {
    if (!error) {
      error_panic();
    }
    const FileLocation loc = error->location;
    bool panic = error->panic_on_emit;
    *error = it->error;
    error->location = loc;
    if (panic) {
      error_panic();
    }
  });
  while (it->_stack != NULL) {
    FsNode *current = it->_stack;
    it->_stack = current->next;
    closedir(current->handle);
    arena_free_chunk(&it->scratch, current);
  }
  arena_free(&it->scratch);
}

bool fs_iter_next(FsIter *it) {
  while (it->_stack != NULL) {
    arena_reset(&it->scratch);
    FsNode *current = it->_stack;

    struct dirent *entry = readdir(current->handle);
    if (entry == NULL) {
      closedir(current->handle);
      it->_stack = current->next;
      arena_free_chunk(&it->scratch, current);
      continue;
    }

    // skip "." and ".." directories
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    Str path = str_format(&it->scratch, "%s/%s", current->name, entry->d_name);

    struct stat entry_info;
    if (stat(path.data, &entry_info) == -1) {
      error_emit(&it->error, errno, "stat failed: %s", strerror(errno));
      return false;
    }

    it->current.path = path;
    it->current.is_dir = S_ISDIR(entry_info.st_mode);

    if (it->current.is_dir && it->recursive) {
      const usize size = sizeof(FsNode) + path.len + 1;
      FsNode *node = arena_calloc_chunk(&it->scratch, size);
      memcpy(node->name, path.data, path.len);
      node->handle = opendir(path.data);
      if (node->handle == NULL) {
        error_emit(&it->error, errno, "opendir failed: %s", strerror(errno));
        return false;
      }
      node->next = it->_stack;
      it->_stack = node;
    }

    return true;
  }

  return false;
}

//////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include <io.h>

typedef struct FsNode {
  HANDLE handle;
  struct FsNode *next;
  usize len;
  char name[];
} FsNode;

bool fs_exists(Path path) {
  char _path[FILENAME_MAX] = {0};
  memcpy(_path, path.data, path.len);
  return _access(_path, 0) == 0;
}

bool fs_is_dir(Path path) {
  char _path[FILENAME_MAX] = {0};
  memcpy(_path, path.data, path.len);
  DWORD attributes = GetFileAttributes(_path);
  if (attributes == INVALID_FILE_ATTRIBUTES) {
    return false;
  }
  if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
    return true;
  }

  return false;
}

FsIter fs_iter_begin(Path directory, bool recursive) {
  FsIter it = {.recursive = recursive, .error = ErrNew};

  const usize len = directory.len + (sizeof("/*") - 1);
  const usize size = sizeof(FsNode) + len + 1;
  FsNode *node = arena_calloc_chunk(&it.scratch, size);
  memcpy(node->name, directory.data, directory.len);
  memcpy(&node->name[directory.len], "/*", 2);
  node->len = len;

  WIN32_FIND_DATA findFileData;
  node->handle = FindFirstFile(node->name, &findFileData);
  if (node->handle == INVALID_HANDLE_VALUE) {
    DWORD err = GetLastError();
    error_emit(&it.error, (i32)err, "FindFirstFile failed (%ld)\n", err);
    return it;
  }

  it._stack = node;

  return it;
}

void fs_iter_end(FsIter *it, Error *error) {
  error_propagate(&it->error, {
    if (!error) {
      error_panic();
    }
    const FileLocation loc = error->location;
    bool panic = error->panic_on_emit;
    *error = it->error;
    error->location = loc;
    if (panic) {
      error_panic();
    }
  });
  while (it->_stack != NULL) {
    FsNode *current = it->_stack;
    it->_stack = current->next;
    FindClose(current->handle);
    arena_free_chunk(&it->scratch, current);
  }
  arena_free(&it->scratch);
}

bool fs_iter_next(FsIter *it) {
  while (it->_stack != NULL) {
    arena_reset(&it->scratch);
    FsNode *current = it->_stack;

    WIN32_FIND_DATA findFileData;
    if (!FindNextFile(current->handle, &findFileData)) {
      FindClose(current->handle);
      it->_stack = current->next;
      arena_free_chunk(&it->scratch, current);
      continue;
    }

    // skip "." and ".." directories
    if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
      continue;
    }

    Str path = str_format(&it->scratch, "%.*s/%s", (i32)current->len - 2, current->name,
                          findFileData.cFileName);

    it->current.path = path;
    it->current.is_dir = findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

    if (it->current.is_dir && it->recursive) {
      const usize len = path.len + (sizeof("/*") - 1);
      const usize size = sizeof(FsNode) + len + 1;
      FsNode *node = arena_calloc_chunk(&it->scratch, size);
      memcpy(node->name, path.data, path.len);
      memcpy(&node->name[path.len], "/*", 2);
      node->len = len;
      node->handle = FindFirstFile(node->name, &findFileData);
      if (node->handle == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        error_emit(&it->error, (i32)err, "FindFirstFile failed (%ld)\n", err);
        return false;
      }
      node->next = it->_stack;
      it->_stack = node;
    }

    return true;
  }

  return false;
}

//////////////////////////////////////////////////////////////////////////////
#endif
