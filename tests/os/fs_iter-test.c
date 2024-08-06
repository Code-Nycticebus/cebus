#include "cebus/os/fs.h"

#include "cebus/collection/da.h"
#include "cebus/core/debug.h"
#include "cebus/core/defines.h"

int main(void) {
  // initialize and configure iterator
  FsIter it = fs_iter_begin(STR("."), true);

  // iterate over directory with certain filters
  while (fs_iter_next_suffix(&it, STR(".toml"))) {
    // every allocation in the scratch buffer gets reset after each iteration
    Str data = fs_file_read_str(it.current.path, &it.scratch, &it.error);

    // do not return before you call 'fs_iter_end'
    error_propagate(&it.error, { break; });

    // do something with data...
    cebus_log(STR_FMT, STR_ARG(data));
  }

  // collect errors and deinitializes iterator
  Error err = ErrNew;
  fs_iter_end(&it, &err);
  error_context(&err, { error_panic(); });
}
