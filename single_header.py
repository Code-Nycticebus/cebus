from pathlib import Path

output = "clib.h"
src = "src"

FIRST = [Path("src/clib/core/platform.h"), Path("src/clib/core/defines.h"), Path("src/clib/core/arena.h")]

def copy(file, src):
    with open(src, "r") as f:
        for line in f.readlines():
            if not line.startswith("#include \""):
               file.write(line)
        file.write("\n")

if __name__ == "__main__":
    with open(output, "w") as f:
        f.write("""/* DOCUMENTATION
Do this:
```c
#define CLIB_IMPLEMENTATION
#include "clib.h"
```
before you include this file in *one* C file to create the implementation.
*/
""")

        for first in FIRST:
            copy(f, first)

        for header in sorted(Path(src).rglob("*.h")):
            if header != Path("src/clib.h") or header not in FIRST:
                copy(f, header)
        
        f.write("#ifdef CLIB_IMPLEMENTATION\n")
        for source in Path(src).rglob("*.c"):
            copy(f, source)
        f.write("#endif // !CLIB_IMPLEMENTATION\n")