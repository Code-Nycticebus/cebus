#include "cebus/os/fs.h"

#include "cebus/collection/da.h"
#include "cebus/core/debug.h"
#include "cebus/core/defines.h"
#include "cebus/type/string.h"

int main(void) {
  fs_iter(it, STR("src"), true, ErrPanic) {
    if (!it.current.is_dir && str_endswith(it.current.path, STR(".c"))) {
      Str data = fs_file_read_str(it.current.path, &it.scratch, it.error);
      error_propagate(it.error, { continue; });
      cebus_log_debug(STR_FMT, STR_ARG(data));
    }
  }
}
