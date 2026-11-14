```c run id=tz-and-localtime
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void show(time_t t) {
    char buf[64];
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M%z %Z", localtime(&t));
    puts(buf);
}

int main(void) {
    // A process has one local zone, which the variable TZ sets.
    setenv("TZ", "America/New_York", 1);
    tzset();
    show(1704801600); // 2024-01-09T12:00:00Z
    show(1720526400); // 2024-07-09T12:00:00Z

    // In an overlap, tm_isdst chooses one of the two instants.
    struct tm a = {.tm_year = 124, .tm_mon = 10, .tm_mday = 3, .tm_hour = 1, .tm_min = 30, .tm_isdst = 1};
    struct tm b = a;
    b.tm_isdst = 0;
    time_t ta = mktime(&a);
    time_t tb = mktime(&b);
    printf("%lld\n", (long long)(tb - ta));

    // A POSIX TZ string states the rules directly, with no database.
    setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1);
    tzset();
    show(1720526400);
    return 0;
}
```

```output
2024-01-09T07:00-0500 EST
2024-07-09T08:00-0400 EDT
3600
2024-07-09T08:00-0400 EDT
```

- Verdict: stdlib. C has only the local zone of the process. POSIX sets it with the variable TZ, which holds a name of the IANA database or a rule string, and localtime and mktime convert with it. There is no zone value that a program can pass to a function.
