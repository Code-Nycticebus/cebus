from pathlib import Path

output = "cebus.h"
src = "src"

FIRST = [
    Path("src/cebus/core/platform.h"),
    Path("src/cebus/core/defines.h"),
    Path("src/cebus/core/arena.h"),
    Path("src/cebus/core/debug.h"),
]


def copy(file, src):
    with open(src, "r") as f:
        for line in f.readlines():
            if line.startswith('#include "'):
                file.write("// ")
            file.write(line)

        file.write("\n")


if __name__ == "__main__":
    with open(output, "w") as f:
        f.write("/* ")
        copy(f, Path("LICENSE"))
        f.write("*/\n")

        f.write(
            """
/* DOCUMENTATION
In your C source file, include the library header and define the implementation
as follows:
```c
#define CEBUS_IMPLEMENTATION
#include "cebus.h"
```
*/

"""
        )

        for first in FIRST:
            copy(f, first)

        for header in sorted(Path(src).rglob("*.h")):
            if header.name != "cebus.h" and header not in FIRST:
                copy(f, header)

        f.write("#ifdef CEBUS_IMPLEMENTATION\n")
        for source in sorted(Path(src).rglob("*.c")):
            copy(f, source)
        f.write("#endif /* !CEBUS_IMPLEMENTATION */\n")
