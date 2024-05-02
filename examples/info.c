#include <clib.h>

int main(void) {
  clib_log_info("CLIB version 1.0.0");
  clib_log_info("%s-%s", CLIB_SYSTEM, CLIB_ARCHITECTURE);
  clib_log_info("%s", CLIB_COMPILER);
#if defined(CLIB_64BIT)
  clib_log_info("64-bit");
#else
  clib_log_info("Bit:\t32-bit");
#endif
  clib_log_info("%s endian",
                CLIB_BYTE_ORDER == ENDIAN_LITTLE ? "little" : "big");
}
