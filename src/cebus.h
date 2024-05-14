/* DOCUMENTATION
Include this file to include all the header files that are listed below.
```c
#include <cebus.h>
```
*/

#ifndef __CEBUS_H__
#define __CEBUS_H__

// IWYU pragma: begin_exports

#include "cebus/collection/da.h"
#include "cebus/collection/hashmap.h"
#include "cebus/collection/set.h"
#include "cebus/collection/string_builder.h"

#include "cebus/core/arena.h"
#include "cebus/core/debug.h"
#include "cebus/core/error.h"
#include "cebus/core/logging.h"
#include "cebus/core/platform.h"

#include "cebus/os/cmd.h"
#include "cebus/os/dll.h"
#include "cebus/os/fs.h"
#include "cebus/os/io.h"
#include "cebus/os/os.h"

#include "cebus/type/byte.h"
#include "cebus/type/char.h"
#include "cebus/type/float.h"
#include "cebus/type/integer.h"
#include "cebus/type/string.h"
#include "cebus/type/utf8.h"

// IWYU pragma: end_exports

#endif /* !__CEBUS_H__ */
