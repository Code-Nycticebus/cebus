#include "core/defines.h"
#include "type/string.h"

#include <stdio.h>
#include <time.h>

typedef struct {
  i32 year;
  i32 month;
  i32 day;
  i32 hour;
  i32 minute;
  i32 second;
} DateTime;

static DateTime dt_from_timestamp(time_t timestamp) {
  struct tm *local = localtime(&timestamp);
  const i32 start_year = 1900;
  return (DateTime){
      .year = start_year + local->tm_year,
      .month = local->tm_mon + 1,
      .day = local->tm_mday,
      .hour = local->tm_hour,
      .minute = local->tm_min,
      .second = local->tm_sec,
  };
}

static DateTime dt_now(void) { return dt_from_timestamp(time(NULL)); }

static Str dt_to_str(const DateTime *dt, Arena *arena) {
  const usize size = 19;
  char *buffer = arena_alloc(arena, size); // NOLINT
  sprintf(buffer, "%d-%d-%d %02d:%02d:%02d", dt->year, dt->month, dt->day,
          dt->hour, dt->minute, dt->second);
  return str_from_parts(size, buffer);
}

static void dt_dump(const DateTime *dt) {
  Arena arena = {0};
  Str s = dt_to_str(dt, &arena);
  printf(STR_FMT "\n", STR_ARG(s));
}

int main(void) {
  DateTime dt = dt_now();
  dt_dump(&dt);
}
