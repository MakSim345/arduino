#include "print_time.h"

#ifdef YS_TEST_123412399  

void print_time()
{

  // Get the current time and date from the chip
  Time t = rtc.time();

  // Name the day of the week
  memset(day, 0, sizeof(day));  // clear day buffer
  switch (t.day) 
  {
    case 1:
      strcpy(day, "Sunday");
      break;
    case 2:
      strcpy(day, "Monday");
      break;
    case 3:
      strcpy(day, "Tuesday");
      break;
    case 4:
      strcpy(day, "Wednesday");
      break;
    case 5:
      strcpy(day, "Thursday");
      break;
    case 6:
      strcpy(day, "Friday");
      break;
    case 7:
      strcpy(day, "Saturday");
      break;
  }

  // Format the time and date and insert into the temporary buffer
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day,
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time
  Serial.println(buf);
}
  #endif

