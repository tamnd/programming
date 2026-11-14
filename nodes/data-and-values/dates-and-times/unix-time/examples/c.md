```c run id=time-t-seconds
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

// The POSIX expression for seconds since the epoch, from the fields of a struct tm.
static long long posix_seconds(const struct tm *t) {
    long long y = t->tm_year;
    return t->tm_sec + t->tm_min * 60LL + t->tm_hour * 3600LL + t->tm_yday * 86400LL
        + (y - 70) * 31536000LL + ((y - 69) / 4) * 86400LL
        - ((y - 1) / 100) * 86400LL + ((y + 299) / 400) * 86400LL;
}

int main(void) {
    time_t t = 1709985600;
    struct tm *utc = gmtime(&t);
    char buf[32];
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%SZ", utc);
    puts(buf);
    printf("%d %lld\n", utc->tm_yday, posix_seconds(utc));

    // The width of time_t on this system, in bits.
    printf("%zu\n", sizeof(time_t) * 8);

    // A signed 32-bit count wraps to 1901 one second after 2038-01-19T03:14:07Z.
    int64_t next = (int64_t)INT32_MAX + 1;
    int32_t wrapped = (int32_t)(uint32_t)next;
    time_t back = wrapped;
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%SZ", gmtime(&back));
    printf("%" PRId32 " %s\n", wrapped, buf);
    return 0;
}
```

```output
2024-03-09T12:00:00Z
68 1709985600
64
-2147483648 1901-12-13T20:45:52Z
```

- Verdict: stdlib. time gives the current calendar time as a time_t, which is a count of seconds since the epoch on POSIX systems. gmtime converts it to the fields of a struct tm in UTC, and timespec_get gives seconds and nanoseconds.
