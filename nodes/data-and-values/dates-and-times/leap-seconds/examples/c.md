```c run id=timegm-sixty
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(void) {
    /* struct tm allows tm_sec up to 60 for a leap second. */
    struct tm leap;
    memset(&leap, 0, sizeof leap);
    const char *end = strptime("2016-12-31 23:59:60", "%Y-%m-%d %H:%M:%S", &leap);
    printf("%d %d\n", end != NULL, leap.tm_sec);

    /* time_t has no number for 23:59:60, so timegm gives the next second. */
    time_t t = timegm(&leap);
    struct tm next = {0};
    next.tm_year = 2017 - 1900;
    next.tm_mday = 1;
    printf("%lld %d\n", (long long)t, t == timegm(&next));

    char buf[32];
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%SZ", gmtime(&t));
    printf("%s %d\n", buf, strcmp(buf, "2017-01-01T00:00:00Z") == 0);

    struct tm before = {0};
    before.tm_year = 2016 - 1900;
    before.tm_mon = 11;
    before.tm_mday = 31;
    before.tm_hour = 23;
    before.tm_min = 59;
    before.tm_sec = 59;
    printf("%.0f\n", difftime(t, timegm(&before)));
    return 0;
}
```

```output
1 60
1483228800 1
2017-01-01T00:00:00Z 1
1
```

- Verdict: stdlib. `struct tm` allows `tm_sec` up to 60, and `strptime` reads it. A `time_t` follows POSIX, with 86400 seconds in each day, so `timegm` gives the second 60 the number of the next second. `timegm` and `strptime` are POSIX and GNU functions, not ISO C.
