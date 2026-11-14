```c run id=strftime-iso
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    char buf[64];
    struct tm tm;
    time_t t = 1735560000; /* 2024-12-30T12:00:00Z */
    gmtime_r(&t, &tm);
    const char *formats[] = {
        "%Y-%m-%dT%H:%M:%SZ", /* calendar date and time in UTC */
        "%Y%m%dT%H%M%SZ",     /* the same in the basic format */
        "%G-W%V-%u",          /* week date: %G is the week-numbering year */
        "%Y-%j",              /* ordinal date */
        "%F %T",              /* %F is %Y-%m-%d and %T is %H:%M:%S */
    };
    for (int i = 0; i < 5; i++) {
        strftime(buf, sizeof buf, formats[i], &tm);
        printf("%s\n", buf);
    }

    /* %z writes the offset as +hhmm, the basic format, with no colon. */
    setenv("TZ", "Asia/Kolkata", 1);
    tzset();
    time_t u = 1720528205; /* 2024-07-09T12:30:05Z */
    localtime_r(&u, &tm);
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%S%z", &tm);
    printf("%s\n", buf);
    return 0;
}
```

```output
2024-12-30T12:00:00Z
20241230T120000Z
2025-W01-1
2024-365
2024-12-30 12:00:00
2024-07-09T18:00:05+0530
```

- Verdict: stdlib. `strftime` has the conversions for each part of ISO 8601: `%F`, `%T`, `%G`, `%V`, `%u` and `%j`. Its `%z` writes the offset in the basic format, so a text in the extended format needs a colon put in by hand. ISO C has no parser; POSIX adds `strptime`.
