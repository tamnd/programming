```c run id=tz-file
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Days from 1970-01-01 to a date of the proleptic Gregorian calendar. */
static long long days_from_civil(long long y, int m, int d) {
    y -= m <= 2;
    long long era = (y >= 0 ? y : y - 399) / 400;
    long long yoe = y - era * 400;
    long long doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    long long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

/* Prints local time and the offset in seconds. The offset is local time minus UTC, both counted as seconds. */
static void show(time_t t) {
    struct tm tm;
    char buf[64];
    localtime_r(&t, &tm);
    long long local = days_from_civil(tm.tm_year + 1900LL, tm.tm_mon + 1, tm.tm_mday) * 86400
                      + tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;
    long long off = local - (long long)t;
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%S %Z", &tm);
    printf("%s %+lld\n", buf, off);
}

int main(void) {
    /* C reaches the tz database only through TZ: a zone name makes localtime read its compiled file. */
    setenv("TZ", "America/Chicago", 1);
    tzset();
    /* 1880-01-01, 1936-06-01 and 2024-07-01, each at 12:00 UTC. */
    const time_t instants[] = {-2840097600, -1059825600, 1719835200};
    for (int i = 0; i < 3; i++) {
        show(instants[i]);
    }
    /* A link name works the same way. */
    setenv("TZ", "US/Central", 1);
    tzset();
    show(instants[2]);
    return 0;
}
```

```output
1880-01-01T06:09:24 LMT -21036
1936-06-01T07:00:00 EST -18000
2024-07-01T07:00:00 CDT -18000
2024-07-01T07:00:00 CDT -18000
```

- Verdict: stdlib. ISO C has no zone names. On POSIX systems, `TZ` set to a name such as `America/Chicago` makes `localtime` read the compiled file of that zone. The example computes the offset itself, because `%z` of `strftime` does not give the offset of each instant on all systems. The offset -21036 seconds is -5:50:36.
