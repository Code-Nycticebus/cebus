#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static uint64_t random_u64(void) {
  return ((uint64_t)rand() << 32) | (uint64_t)rand();
}
static uint32_t random_u32(void) { return (uint32_t)rand(); }
static uint16_t random_u16(void) { return (uint16_t)rand(); }
static uint8_t random_u8(void) { return (uint8_t)rand(); }

int main(void) {
  srand((uint32_t)time(NULL));
  printf("%lu\n", random_u64());
  printf("%u\n", random_u32());
  printf("%u\n", random_u16());
  printf("%u\n", random_u8());
  printf("\n");
}
