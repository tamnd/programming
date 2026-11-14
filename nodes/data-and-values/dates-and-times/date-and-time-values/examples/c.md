```c run id=time-t-and-tm
#include <stdio.h>
#include <time.h>

int main(void) {
    // A time_t is a count of seconds since the epoch on POSIX systems.
    time_t t = 1709985600;
    struct tm *utc = gmtime(&t);
    // tm_year counts from 1900, and tm_mon counts from 0.
    printf("%d %d %d\n", utc->tm_year, utc->tm_mon, utc->tm_mday);

    char buf[32];
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%SZ", utc);
    puts(buf);

    // mktime normalizes fields that are out of range: February 30 is March 1.
    struct tm feb = {.tm_year = 124, .tm_mon = 1, .tm_mday = 30, .tm_hour = 12, .tm_isdst = -1};
    mktime(&feb);
    printf("%d-%02d-%02d\n", feb.tm_year + 1900, feb.tm_mon + 1, feb.tm_mday);

    // difftime gives the difference of two time_t values in seconds.
    printf("%.0f\n", difftime(t + 5400, t));
    return 0;
}
```

```output
124 2 9
2024-03-09T12:00:00Z
2024-03-01
5400
```

- Verdict: stdlib. time.h has time_t for a calendar time, struct tm for its fields, and time, gmtime, localtime, mktime and strftime. The fields of struct tm start at 1900 for the year and at 0 for the month. There are no zone names other than the local zone of the process.
