#include "dll.h"

#include "cebus/core/platform.h"
#include "cebus/os/fs.h"
#include "cebus/type/integer.h"
#include "cebus/type/string.h"

#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)
#include <dlfcn.h>

Dll *dll_load(Path path, Error *error) {
  if (!fs_exists(path)) {
    error_emit(error, FS_NOT_FOUND, "dll: library does not exist: " STR_FMT, STR_ARG(path));
    return NULL;
  }
  char lib_path[FILENAME_MAX] = {0};
  memcpy(lib_path, path.data, usize_min(path.len, FILENAME_MAX));

  Dll *handle = dlopen(lib_path, RTLD_LAZY);
  if (handle == NULL) {
    error_emit(error, -1, "dll: %s: %s\n", lib_path, dlerror());
    return NULL;
  }
  return handle;
}

void dll_close(Dll *handle) { dlclose(handle); }

Function dll_symbol(Dll *handle, Str symbol, Error *error) {
  Arena arena = {0};
  symbol = str_copy(symbol, &arena);

  Function fn;
  *(void **)(&fn) = dlsym(handle, symbol.data);
  if (fn == NULL) {
    error_emit(error, -1, "dll function: %s: %s\n", symbol.data, dlerror());
    goto defer;
  }

defer:
  arena_free(&arena);
  return fn;
}

//////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include <windows.h>

Dll *dll_load(Str path, Error *error) {
  if (!fs_exists(path)) {
    error_emit(error, -1, "dll: library does not exist: " STR_FMT, STR_ARG(path));
    return NULL;
  }
  char lib_path[FILENAME_MAX] = {0};
  memcpy(lib_path, path.data, usize_min(path.len, FILENAME_MAX));

  char temp_path[MAX_PATH];
  GetTempPath(MAX_PATH, temp_path);

  char temp_file_name[MAX_PATH];
  GetTempFileName(temp_path, TEXT("lib"), 0, temp_file_name);
  CopyFile(lib_path, temp_file_name, 0);

  HINSTANCE handle = LoadLibraryA(temp_file_name);
  if (handle == NULL) {
    DWORD ec = GetLastError();
    error_emit(error, (i32)ec, "error loading library: " STR_FMT ": %lu\n", STR_ARG(path),
               GetLastError());
    return NULL;
  }
  return handle;
}

void dll_close(Dll *handle) {
  char temp_file_name[MAX_PATH];
  GetModuleFileName(handle, temp_file_name, MAX_PATH);
  FreeLibrary(handle);
  DeleteFile(temp_file_name);
}

Function dll_symbol(Dll *handle, Str symbol, Error *error) {
  Arena arena = {0};
  symbol = str_copy(symbol, &arena);
  Function fn = (Function)GetProcAddress(handle, symbol.data);
  if (fn == NULL) {
    DWORD err_code = GetLastError();
    error_emit(error, (i32)err_code, "dll function: %s: %lu\n", symbol.data, err_code);
    goto defer;
  }

defer:
  arena_free(&arena);
  return fn;
}

//////////////////////////////////////////////////////////////////////////////
#endif
