---
tag: HBV3KC
kind: task
slug: get-a-unix-timestamp
name: Get a Unix timestamp
aliases: [unix timestamp, epoch time, epoch seconds, epoch milliseconds, seconds since the epoch, posix time, current timestamp, date to timestamp, timestamp to date]
area: data-and-values/dates-and-times
level: intro
summary: Get the seconds or milliseconds since 1970-01-01T00:00:00Z as an integer, and convert a date, a time and a UTC offset to that number and back without the local zone of the system.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap04.html
    title: The Open Group Base Specifications Issue 8, General Concepts
    locator: 4.19 Seconds Since the Epoch
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/time.html
    title: The Open Group Base Specifications Issue 8, time
    locator: DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/mktime.html
    title: The Open Group Base Specifications Issue 8, mktime
    locator: DESCRIPTION
  - url: https://docs.python.org/3/library/time.html
    title: The Python Standard Library, time
    locator: time.time, time.time_ns, time.mktime
  - url: https://docs.python.org/3/library/datetime.html
    title: The Python Standard Library, datetime
    locator: datetime.timestamp, datetime.fromtimestamp
  - url: https://docs.python.org/3/library/calendar.html
    title: The Python Standard Library, calendar
    locator: calendar.timegm
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date
    title: MDN Web Docs, Date
    locator: The epoch, timestamps, and invalid date
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/UTC
    title: MDN Web Docs, Date.UTC()
    locator: Parameters, year
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/now
    title: MDN Web Docs, Date.now()
    locator: Return value; Reduced time precision
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/Instant.html
    title: Java SE 25 API, java.time.Instant
    locator: getEpochSecond(); toEpochMilli(); ofEpochSecond(long); ofEpochMilli(long)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/OffsetDateTime.html
    title: Java SE 25 API, java.time.OffsetDateTime
    locator: toEpochSecond()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.html
    title: Java SE 25 API, java.lang.System
    locator: currentTimeMillis()
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.datetimeoffset.tounixtimeseconds?view=net-10.0
    title: .NET API reference, DateTimeOffset.ToUnixTimeSeconds
    locator: Returns; Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.datetimeoffset.fromunixtimeseconds?view=net-10.0
    title: .NET API reference, DateTimeOffset.FromUnixTimeSeconds
    locator: Exceptions; Remarks
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: func Unix; func UnixMilli; func (Time) Unix; func (Time) UnixMilli; func FixedZone
  - url: https://doc.rust-lang.org/std/time/struct.SystemTime.html
    title: The Rust Standard Library, std::time::SystemTime
    locator: duration_since; UNIX_EPOCH; Examples
  - url: https://en.cppreference.com/w/c/chrono/timegm.html
    title: cppreference.com, timegm
    locator: the description of the function; Return value
  - url: https://en.cppreference.com/w/cpp/chrono/system_clock.html
    title: cppreference.com, std::chrono::system_clock
    locator: the description of the class
  - url: https://en.cppreference.com/w/cpp/chrono/year_month_day.html
    title: cppreference.com, std::chrono::year_month_day
    locator: the description of the class; operator sys_days
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual
    locator: "5.2 Bash Variables: EPOCHSECONDS, EPOCHREALTIME"
  - url: https://man7.org/linux/man-pages/man1/date.1.html
    title: Linux manual page, date(1)
    locator: "FORMAT: %s; EXAMPLES: convert seconds since the Epoch"
---

## Task

Get a Unix timestamp: the number of seconds since 1970-01-01T00:00:00Z, without leap seconds. Many programs and formats store a time as this integer, or as the number of milliseconds since the same instant. Get the timestamp of the current time, and convert a date and a time of day to a timestamp and back.

The real time changes on each run, so the contract tests the conversion. `unix_timestamp(year, month, day, hour, minute, second, offset_minutes)` takes a local date and time and its UTC offset in minutes, positive east of Greenwich, and returns the timestamp in seconds. The offset is part of the input, so the result does not depend on the time zone of the system. A time before 1970 gives a negative timestamp. The years are 1 to 9999 in the proleptic Gregorian calendar.

Each solution also has `now_unix_seconds()` and `now_unix_millis()`. They read the system clock and round toward the past. The tests check that both values are in 2024 or later and that they agree. The tests also convert each expected timestamp back to a date with the date type of the language.

## Solutions

POSIX defines "Seconds Since the Epoch" with a formula from the fields of a UTC date. Each day has exactly 86400 seconds in this count, so a leap second has no number of its own (see [[leap-seconds]]). Each solution below keeps the timestamp as an integer. A timestamp in seconds is in the range of a signed 64-bit integer for each year of the contract.

- **Python.** `time.time()` returns the current time in seconds as a `float`, and `time.time_ns()` returns it in nanoseconds as an `int`. `time.time_ns() // 1_000_000_000` gives whole seconds, rounded toward the past. `datetime.timestamp()` also returns a `float`, and it treats a naive `datetime` as local time. The solution subtracts the epoch from an aware `datetime` and divides by `timedelta(seconds=1)`, which gives an `int`. `calendar.timegm(t)` converts a UTC time tuple to a timestamp, and `datetime.fromtimestamp(s, timezone.utc)` converts back.
- **JavaScript.** A `Date` holds the number of milliseconds since the epoch, and `Date.now()` and `getTime()` return it. Divide by 1000 and use `Math.floor` to get whole seconds. `Date.UTC(year, monthIndex, ...)` reads the years 0 to 99 as 1900 to 1999, and its month starts at 0. The solution calls `setUTCFullYear` and `setUTCHours` on a `Date`, which take the year as it is. The minutes argument can be negative or over 59, so it also subtracts the offset. `new Date(seconds * 1000)` converts back.
- **TypeScript.** The same code as JavaScript, with the type `number`.
- **Java.** `Instant.now().getEpochSecond()` gives whole seconds, and `System.currentTimeMillis()` and `Instant.toEpochMilli()` give milliseconds. `OffsetDateTime.of(...).toEpochSecond()` converts a date, a time and a `ZoneOffset` to seconds. `Instant.ofEpochSecond(s)` converts back, and `atOffset(offset)` gives the local date and time again.
- **C#.** `DateTimeOffset.UtcNow.ToUnixTimeSeconds()` and `ToUnixTimeMilliseconds()` give the current timestamp. `new DateTimeOffset(year, month, day, hour, minute, second, offset)` holds the offset, and `ToUnixTimeSeconds()` converts it to UTC first. `DateTimeOffset.FromUnixTimeSeconds(s)` converts back, and it accepts -62,135,596,800 to 253,402,300,799: the years 1 to 9999.
- **Go.** `time.Now().Unix()` and `time.Now().UnixMilli()` give the current timestamp. `time.Date(year, month, day, hour, minute, second, 0, loc)` with `time.FixedZone("", offset)` makes a time with a fixed offset, and `Unix()` converts it. `time.Unix(s, 0)` and `time.UnixMilli(ms)` convert back.
- **Rust.** `SystemTime::now().duration_since(UNIX_EPOCH)` returns a `Duration`, or an error if the clock is before 1970. `as_secs()` and `as_millis()` give whole units. The standard library has no calendar type, so the solution converts the date to days with the `days_from_civil` algorithm of Howard Hinnant. A crate such as `chrono` or `jiff` has calendar types.
- **C.** `time(NULL)` returns the current time in seconds since the epoch as a `time_t`, and `timespec_get(&ts, TIME_UTC)` adds nanoseconds. `mktime` reads a `struct tm` as local time. `timegm` reads it as UTC, and C23 added it to the standard. Both work in `time_t`, and the range of `time_t` and of these functions depends on the system. The solution computes the days with `days_from_civil` and needs only 64-bit integers. `gmtime_r` converts a timestamp back to a `struct tm`.
- **C++.** `std::chrono::system_clock` measures Unix time. `floor<seconds>(system_clock::now()).time_since_epoch().count()` gives whole seconds. A `year_month_day` converts to `sys_days`, a count of days since the epoch, and the solution adds the hours, the minutes and the seconds to it. `std::format("{:%FT%T}", t)` writes a `sys_seconds` back as a date.
- **Bash.** `$EPOCHSECONDS` expands to the current time in whole seconds, and `$EPOCHREALTIME` to seconds with microseconds. `date +%s` prints the same number. GNU `date -u -d '2024-03-10T02:00:00-05:00' +%s` converts a date with an offset, and `date -u -d @1710054000` converts back.

A timestamp is the same number in each time zone. Only its conversion to a date and a time of day needs a zone:

```python run id=python-same-number
from datetime import datetime, timedelta, timezone

EPOCH = datetime(1970, 1, 1, tzinfo=timezone.utc)
new_york = datetime(2024, 3, 10, 2, 0, tzinfo=timezone(timedelta(hours=-5)))
tokyo = datetime(2024, 3, 10, 16, 0, tzinfo=timezone(timedelta(hours=9)))
print((new_york - EPOCH) // timedelta(seconds=1))
print((tokyo - EPOCH) // timedelta(seconds=1))
print(datetime.fromtimestamp(1710054000, timezone.utc).isoformat())
```

```output for=python-same-number
1710054000
1710054000
2024-03-10T07:00:00+00:00
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `epoch` and `billennium`: 1970-01-01T00:00:00Z is 0, and 2001-09-09T01:46:40Z is 1000000000.
- `negative-offset` and `positive-offset`: 2024-03-10T02:00:00-05:00 and 2024-03-10T16:00:00+09:00 are the same instant, 1710054000.
- `one-second-before-epoch` and `offset-moves-before-epoch`: 1969-12-31T23:59:59Z is -1, and 1970-01-01T00:00:00+01:00 is -3600.
- `offset-moves-to-next-year`, `offset-of-45-minutes` and `offset-of-minus-9-30`: an offset can move the UTC time to another year, and an offset can have 30 or 45 minutes.
- `offset-plus-14` and `offset-minus-12`: the offsets +14:00 and -12:00 also give 0 at the epoch.
- `leap-day`, `year-2000-is-leap` and `year-1900-is-not-leap`: the year 1900 has no February 29, so 1900-03-01 is -2203891200.
- `int32-max`, `after-int32-max` and `int32-min`: 2038-01-19T03:14:07Z is 2147483647, the largest value of a signed 32-bit integer. One second later needs more than 32 bits. 1901-12-13T20:45:52Z is -2147483648.
- `year-99-is-not-1999`: the year 99 is -59011459201. `Date.UTC` in JavaScript gives the year 1999 for it.
- `first-second-of-year-1` and `last-second-of-year-9999`: -62135596800 and 253402300799, the range of .NET `DateTimeOffset`.

## Pitfalls

- **Seconds and milliseconds are both called a timestamp.** JavaScript, Java `currentTimeMillis` and Go `UnixMilli` use milliseconds. Most of the other functions on this page use seconds. A value in the wrong unit gives a date near 1970 or a date thousands of years later:

```javascript wrong id=js-seconds-as-millis
const seconds = 1710054000;
console.log(new Date(seconds).toISOString());
console.log(new Date(seconds * 1000).toISOString());
console.log(Date.UTC(99, 11, 31) / 1000);
```

```output for=js-seconds-as-millis
1970-01-20T19:00:54.000Z
2024-03-10T07:00:00.000Z
946598400
```

The last line also shows that `Date.UTC` reads the year 99 as 1999.

```go wrong id=go-millis-as-seconds
package main

import (
	"fmt"
	"time"
)

func main() {
	millis := int64(1710054000123)
	fmt.Println(time.Unix(millis, 0).UTC().Year())
	fmt.Println(time.UnixMilli(millis).UTC().Format(time.RFC3339Nano))
}
```

```output for=go-millis-as-seconds
56159
2024-03-10T07:00:00.123Z
```

- **`mktime` and naive values use the local zone.** C `mktime`, Python `time.mktime` and Python `datetime.timestamp()` with a naive `datetime` read the fields as local time. The result changes with the time zone of the system. Use `calendar.timegm`, or an aware `datetime`:

```python wrong id=python-mktime-local
import calendar
import os
import time

os.environ["TZ"] = "America/New_York"
time.tzset()

fields = (2024, 1, 1, 0, 0, 0, 0, 1, 0)
print("mktime:", time.mktime(fields))
print("timegm:", calendar.timegm(fields))
```

```output for=python-mktime-local
mktime: 1704085200.0
timegm: 1704067200
```

The same mistake in Bash: `date -d` reads a date with no offset as local time.

```bash wrong id=bash-local-date
TZ=America/New_York date -d '2024-01-01 00:00' +%s
TZ=America/New_York date -d '2024-01-01T00:00:00Z' +%s
date -u -d '2024-01-01 00:00' +%s
```

```output for=bash-local-date
1704085200
1704067200
1704067200
```

- **A `float` of seconds loses digits.** A 64-bit `float` has about 16 significant decimal digits, and a timestamp in seconds already uses 10 of them. It cannot hold nanoseconds. Keep an integer, such as the result of `time.time_ns()`:

```python wrong id=python-float-seconds
ns = 1710054000123456789
seconds = ns / 1e9
print(seconds)
print(int(seconds * 1e9))
print(divmod(ns, 10**9))
```

```output for=python-float-seconds
1710054000.1234567
1710054000123456768
(1710054000, 123456789)
```

- **Integer division of a negative timestamp goes toward zero.** In Java, C, C++, C#, Go and Rust, `/` on integers drops the fraction toward zero. For a time before 1970, milliseconds divided by 1000 give the next second, not the second that contains the time. Use `Math.floorDiv`, `div_euclid`, or a library function:

```java wrong id=java-negative-millis
import java.time.Instant;

public class Main {
    public static void main(String[] args) {
        long millis = -1500L; // 1969-12-31T23:59:58.500Z
        System.out.println(millis / 1000);
        System.out.println(Math.floorDiv(millis, 1000L));
        System.out.println(Instant.ofEpochMilli(millis).getEpochSecond());
    }
}
```

```output for=java-negative-millis
-1
-2
-2
```

`ToUnixTimeSeconds` in .NET returns the second that contains the time:

```csharp run id=csharp-before-epoch
var t = new DateTimeOffset(1969, 12, 31, 23, 59, 58, 500, TimeSpan.Zero);
Console.WriteLine(t.ToUnixTimeMilliseconds());
Console.WriteLine(t.ToUnixTimeSeconds());
Console.WriteLine(t.ToUnixTimeMilliseconds() / 1000);
```

```output for=csharp-before-epoch
-1500
-2
-1
```

- **A 32-bit timestamp ends in 2038.** A signed 32-bit integer holds the timestamps up to 2038-01-19T03:14:07Z. One second later, the bits of the next value are the bits of -2147483648, which is a time in 1901. Use a 64-bit type for a timestamp, also in files and in database columns:

```c wrong id=c-int32-overflow
#define _DEFAULT_SOURCE
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(void) {
    uint32_t bits = UINT32_C(2147483648); // 2038-01-19T03:14:08Z
    int32_t stored;
    memcpy(&stored, &bits, sizeof stored);
    time_t t = stored;
    struct tm tm;
    char text[32];
    gmtime_r(&t, &tm);
    strftime(text, sizeof text, "%Y-%m-%dT%H:%M:%SZ", &tm);
    printf("%" PRId32 " %s\n", stored, text);
}
```

```output for=c-int32-overflow
-2147483648 1901-12-13T20:45:52Z
```

- **A timestamp has no leap seconds.** A difference of two timestamps across a leap second is one second short of the real elapsed time. Measure a duration with a monotonic clock (see [[measure-elapsed-time]]).

## Related tasks

- [[get-the-current-date-and-time]]: read the clock and write the instant as an ISO 8601 timestamp.
- [[parse-and-format-a-date]]: read a date from text, and write it as text.
- [[convert-between-time-zones]]: show a timestamp in the time zone of a person.
- [[compute-the-difference-of-two-dates]]: subtract two dates.

See [[unix-time]] for the definition and the history of the count.

## References
