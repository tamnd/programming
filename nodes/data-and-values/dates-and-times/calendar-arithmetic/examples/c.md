```c run id=mktime-normalize
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void show(const struct tm *tm) {
    char buf[40];
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M %Z", tm);
    printf("%s\n", buf);
}

int main(void) {
    setenv("TZ", "America/New_York", 1);
    tzset();

    /* mktime normalizes fields that are out of range: February 31 is March 2. */
    struct tm d = {0};
    d.tm_year = 2024 - 1900;
    d.tm_mon = 0;
    d.tm_mday = 31;
    d.tm_hour = 12;
    d.tm_isdst = -1;
    d.tm_mon += 1;
    mktime(&d);
    show(&d);

    /* Add a calendar day in local time, with tm_isdst = -1 so mktime finds the offset. */
    struct tm t = {0};
    t.tm_year = 2024 - 1900;
    t.tm_mon = 2;
    t.tm_mday = 9;
    t.tm_hour = 18;
    t.tm_isdst = -1;
    time_t start = mktime(&t);
    struct tm day = t;
    day.tm_mday += 1;
    day.tm_isdst = -1;
    time_t next = mktime(&day);
    show(&day);
    printf("%.0f\n", difftime(next, start) / 3600);

    /* Add 86400 seconds to the time_t. */
    time_t exact = start + 86400;
    struct tm e;
    localtime_r(&exact, &e);
    show(&e);
    return 0;
}
```

```output
2024-03-02 12:00 EST
2024-03-10 18:00 EDT
23
2024-03-10 19:00 EDT
```

- Verdict: stdlib. A program adds to the fields of a `struct tm`, and `mktime` normalizes the values that are out of range, so February 31 becomes March 2. With `tm_isdst` set to -1, `mktime` finds the offset of the new local time. Adding 86400 to a `time_t` adds 24 exact hours.
