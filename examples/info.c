#include <cebus.h>

int main(void) {
  cebus_log_info("CEBUS version 1.0.0");
  cebus_log_info("%s-%s", CEBUS_SYSTEM, CEBUS_ARCHITECTURE);
  cebus_log_info("%s", CEBUS_COMPILER);
#if defined(CEBUS_64BIT)
  cebus_log_info("64-bit");
#else
  cebus_log_info("Bit:\t32-bit");
#endif
  cebus_log_info("%s endian",
                CEBUS_BYTE_ORDER == ENDIAN_LITTLE ? "little" : "big");
}
