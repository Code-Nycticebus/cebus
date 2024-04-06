/* DOCUMENTATION
Include this file to include all the header files that are listed below.
```c
#include <clib/clib.h>
```
*/

#ifndef __CLIB_H__
#define __CLIB_H__

// IWYU pragma: begin_exports

#include "clib/collection/da.h"
#include "clib/collection/hashmap.h"
#include "clib/collection/set.h"
#include "clib/collection/string_builder.h"
#include "clib/core/arena.h"
#include "clib/core/assert.h"
#include "clib/core/error.h"
#include "clib/core/logging.h"
#include "clib/core/platform.h"
#include "clib/os/cmd.h"
#include "clib/os/dll.h"
#include "clib/os/fs.h"
#include "clib/os/io.h"
#include "clib/os/os.h"
#include "clib/type/byte.h"
#include "clib/type/char.h"
#include "clib/type/float.h"
#include "clib/type/integer.h"
#include "clib/type/string.h"
#include "clib/type/utf8.h"

// IWYU pragma: end_exports

#endif /* !__CLIB_H__ */
