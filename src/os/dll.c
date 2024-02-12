#include "dll.h"

#include "core/platform.h"
#include "os/fs.h"
#include "type/integer.h"

#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)
#include <dlfcn.h>

Dll *dll_load(Str path, Error *error) {
  if (!file_exists(path)) {
    error_emit(error, -1, "dll: library does not exist: " STR_FMT,
               STR_ARG(path));
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

Function *dll_symbol(Dll *handle, const char *symbol, Error *error) {
  Function *fn = dlsym(handle, symbol);
  if (fn == NULL) {
    error_emit(error, -1, "dll function: %s: %s\n", symbol, dlerror());
    return NULL;
  }
  return fn;
}

//////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include <windows.h>

Dll *dll_load(Str path, Error *error) {
  if (!file_exists(path)) {
    error_emit(error, -1, "dll: library does not exist: " STR_FMT,
               STR_ARG(path));
    return NULL;
  }
  char lib_path[FILENAME_MAX] = {0};
  memcpy(lib_path, path.data, usize_min(path.len, FILENAME_MAX));

  char temp_path[MAX_PATH];
  GetTempPath(MAX_PATH, temp_path);

  char temp_file_name[MAX_PATH];
  GetTempFileName(temp_path, TEXT("lib"), 0, temp_file_name);
  CopyFile(lib_path, temp_file_name, 0));

  HINSTANCE handle = LoadLibraryA(temp_file_path);
  if (handle == NULL) {
    error_emit(error, -1, "error loading library: %s: %s\n", lib,
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

Function *dll_symbol(Dll *handle, const char *symbol, Error *error) {
  Function *fn = (void *)GetProcAddress(handle, symbol);
  if (fn == NULL) {
    error_emit(error, -1, "dll function: %s: %s\n", symbol, GetLastError());
    return NULL;
  }
  return fn;
}

//////////////////////////////////////////////////////////////////////////////
#endif
