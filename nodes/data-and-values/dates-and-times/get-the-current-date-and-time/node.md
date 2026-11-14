---
tag: 7R7Z7P
kind: task
slug: get-the-current-date-and-time
name: Get the current date and time
aliases: [now, current time, current date, get the time, timestamp now, utc now, local time now, read the system clock, datetime now]
area: data-and-values/dates-and-times
level: intro
summary: Read the system clock once, keep the reading as an instant, and write it in UTC as an ISO 8601 timestamp with milliseconds. Convert to local time only to show it to a person.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/datetime.html
    title: The Python Standard Library, datetime
    locator: datetime.now, datetime.utcnow, Aware and naive objects
  - url: https://docs.python.org/3/library/time.html
    title: The Python Standard Library, time
    locator: time.time, time.time_ns
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/now
    title: MDN Web Docs, Date.now()
    locator: Description, Reduced time precision
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/toISOString
    title: MDN Web Docs, Date.prototype.toISOString()
    locator: Description
  - url: https://tc39.es/proposal-temporal/docs/now.html
    title: TC39 Temporal proposal, Temporal.Now
    locator: Temporal.Now.instant, Temporal.Now.zonedDateTimeISO, Temporal.Now.timeZoneId
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/Clock.html
    title: Java SE 25 API, java.time.Clock
    locator: Class description; systemUTC(); fixed(Instant, ZoneId)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/Instant.html
    title: Java SE 25 API, java.time.Instant
    locator: now(); toString(); truncatedTo(TemporalUnit)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.datetimeoffset.utcnow?view=net-10.0
    title: .NET API reference, DateTimeOffset.UtcNow
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.datetime.now?view=net-10.0
    title: .NET API reference, DateTime.Now
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.timeprovider?view=net-10.0
    title: .NET API reference, TimeProvider
    locator: Remarks; Properties (System); Methods (GetUtcNow, GetLocalNow)
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: func Now; Monotonic Clocks; Constants (layouts); func (Time) Equal
  - url: https://doc.rust-lang.org/std/time/struct.SystemTime.html
    title: The Rust Standard Library, std::time::SystemTime
    locator: Description; now; duration_since; UNIX_EPOCH
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/clock_gettime.html
    title: The Open Group Base Specifications Issue 8, clock_getres, clock_gettime, clock_settime
    locator: DESCRIPTION, CLOCK_REALTIME
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/gmtime.html
    title: The Open Group Base Specifications Issue 8, gmtime, gmtime_r
    locator: DESCRIPTION; RETURN VALUE
  - url: https://en.cppreference.com/w/c/chrono/timespec_get.html
    title: cppreference.com, timespec_get
    locator: the description of the function
  - url: https://en.cppreference.com/w/cpp/chrono/system_clock.html
    title: cppreference.com, std::chrono::system_clock
    locator: the description of the class
  - url: https://en.cppreference.com/w/cpp/chrono/system_clock/formatter.html
    title: cppreference.com, std::formatter<std::chrono::sys_time>
    locator: Format specification, %S and %T
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual
    locator: "5.2 Bash Variables: EPOCHREALTIME, EPOCHSECONDS; 4.2 Bash Builtin Commands: printf, %(datefmt)T"
  - url: https://man7.org/linux/man-pages/man1/date.1.html
    title: Linux manual page, date(1)
    locator: DESCRIPTION; EXAMPLES
---

## Task

Get the current date and time. Read the system clock once, and keep the reading as an instant: a point on the time line with no time zone. Write it in UTC as an ISO 8601 timestamp, such as `2024-03-10T07:00:00.123Z`. Convert it to the local time of a person only to show it to that person.

The real time changes on each run, so the contract tests the conversion. `utc_timestamp(epoch_ns)` takes one reading of the system clock in nanoseconds since 1970-01-01T00:00:00Z, without leap seconds, and returns `YYYY-MM-DDTHH:MM:SS.mmmZ`. The result always has three digits of milliseconds. The part under one millisecond is dropped toward the past, so a reading before 1970 goes to the earlier millisecond. The reading can be any signed 64-bit integer, which covers the years 1677 to 2262.

Each solution also has `now_utc(clock)`. It reads the clock once and returns `utc_timestamp` of the reading. Its tests give a fake clock with a fixed reading and check the result. One more test uses the real clock and checks only the shape of the result and that the year is 2024 or later.

## Solutions

Each solution keeps the reading as an integer and divides with a rule that goes toward the past. A division that goes toward zero puts the readings from -999999 ns to -1 ns at 1970-01-01T00:00:00.000Z, not at 1969-12-31T23:59:59.999Z.

- **Python.** `time.time_ns()` returns an `int` of nanoseconds since the epoch. `time.time()` returns a `float`, and the documentation says to use `time_ns()` to avoid the precision loss of `float`. `datetime.now(timezone.utc)` returns an aware `datetime` in UTC, and `datetime.now().astimezone()` returns the local time with its offset. `datetime.utcnow()` is deprecated: it returns a naive `datetime`, which many methods treat as local time (see the pitfalls). The solution adds a `timedelta` of whole milliseconds to the epoch. The `//` and `%` of Python go toward the past.
- **JavaScript.** `Date.now()` returns a `number` of milliseconds since the epoch. Browsers can reduce its precision to protect privacy. `new Date().toISOString()` always writes UTC with three digits of milliseconds, in 24 characters for the years 0 to 9999. The `/` of `bigint` goes toward zero, so the solution steps back one millisecond for a negative rest. Where the Temporal API is available, `Temporal.Now.instant()` returns an instant with nanoseconds, and `Temporal.Now.zonedDateTimeISO()` returns the time in the zone of the system.
- **TypeScript.** The same code as JavaScript, with the types `bigint` and `() => bigint`.
- **Java.** `Clock.systemUTC().instant()` and `Instant.now()` read the system clock. The documentation of `Clock` says to pass a `Clock` to each method that needs the current instant, so that a test can give `Clock.fixed(...)`. `ZonedDateTime.now()` gives the time in the default zone of the JVM. `Instant.toString()` writes zero, three, six or nine digits of the fraction, as needed, so the solution uses a pattern with `SSS`. `Math.floorDiv` and `Math.floorMod` go toward the past.
- **C#.** `DateTimeOffset.UtcNow` gives the current UTC time. `DateTime.Now` gives the local time with `Kind` set to `Local`, and `DateTime.UtcNow` gives UTC. The resolution depends on the system timer, and the documentation of `DateTime.Now` gives 0.5 to 15 ms. `TimeProvider.System.GetUtcNow()` reads the same clock, and a test gives a subclass of `TimeProvider` that overrides `GetUtcNow`. A tick of `DateTimeOffset` is 100 ns. The format uses the invariant culture.
- **Go.** `time.Now()` returns the current local time: a `Time` with a wall clock reading, a monotonic clock reading and the local `Location`. `UnixNano()` gives the nanoseconds since the epoch, and `UTC()` sets the location to UTC. In the layout `2006-01-02T15:04:05.000Z07:00`, `.000` writes three digits of the fraction and drops the rest, and `Z07:00` writes `Z` for UTC. `time.Unix(0, ns)` keeps the nanoseconds in `[0, 1e9)`, so a negative reading goes toward the past.
- **Rust.** `SystemTime::now()` reads the system clock. It is not monotonic. `duration_since(UNIX_EPOCH)` returns a `Duration`, or an error for a time before 1970. The standard library cannot write a calendar date, so the solution has the `civil_from_days` algorithm of Howard Hinnant, which converts a count of days to a year, a month and a day. A program that needs time zones uses a crate, such as `chrono` or `jiff`. `div_euclid` and `rem_euclid` go toward the past.
- **C.** `timespec_get(&ts, TIME_UTC)` fills a `struct timespec` with the calendar time. On a POSIX system, `clock_gettime(CLOCK_REALTIME, &ts)` reads the same clock. `gmtime_r` converts seconds to a `struct tm` in UTC, and `localtime_r` converts them to local time. `strftime` writes the date, and `snprintf` adds the milliseconds, because `struct tm` has no fraction. The `/` of C goes toward zero, so the code has a `floor_div`.
- **C++.** `std::chrono::system_clock::now()` reads the system clock, and its epoch is Unix time. `floor<milliseconds>` goes toward the past, and `duration_cast` goes toward zero. `std::format("{:%FT%TZ}", t)` writes a `sys_time`, and `%T` writes as many digits of the fraction as the duration has. `std::chrono::zoned_time{std::chrono::current_zone(), now}` gives the local time.
- **Bash.** `EPOCHSECONDS` expands to whole seconds since the epoch, and `EPOCHREALTIME` to seconds with six digits of microseconds. The solution keeps only the digits of `EPOCHREALTIME`, so the character of the decimal point does not matter. `date -u -d @SECONDS` from GNU coreutils converts seconds to a UTC date. `printf '%(%F %T)T' SECONDS` is a builtin that converts seconds to local time with no new process.

A real reading prints a different time on each run. A program that must give the same output can print a check:

```python run id=python-now
import re
from datetime import datetime, timezone

now = datetime.now(timezone.utc)
text = now.isoformat(timespec="milliseconds").replace("+00:00", "Z")
print("aware:", now.tzinfo is not None)
print("shape:", re.fullmatch(r"\d{4}-\d\d-\d\dT\d\d:\d\d:\d\d\.\d{3}Z", text) is not None)
print("year >= 2024:", now.year >= 2024)
```

```output for=python-now
aware: True
shape: True
year >= 2024: True
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `epoch` and `billennium`: 0 ns is `1970-01-01T00:00:00.000Z`, and 10^18 ns is `2001-09-09T01:46:40.000Z`.
- `with-milliseconds` and `truncate-not-round`: 1710054000123456789 ns gives `.123`, not `.124`, and 999999 ns gives `.000`.
- `one-ns-before-epoch`, `one-ms-before-epoch` and `just-over-one-ms-before-epoch`: -1 ns and -1000000 ns both give `1969-12-31T23:59:59.999Z`, and -1000001 ns gives `.998`. A division that goes toward zero gives `1970-01-01T00:00:00.000Z` for -1 ns.
- `last-ms-of-a-year`: 2023-12-31T23:59:59.999999999 gives `2023-12-31T23:59:59.999Z`. Rounding gives the next year.
- `leap-day`, `year-2000-is-leap` and `year-2100-is-not-leap`: the year 2000 has a February 29, and the year 2100 does not.
- `after-int32-seconds`: 2^31 seconds is `2038-01-19T03:14:08.000Z`. A signed 32-bit `time_t` cannot hold it.
- `int64-max` and `int64-min`: the ends of the signed 64-bit range of nanoseconds are `2262-04-11T23:47:16.854Z` and `1677-09-21T00:12:43.145Z`. JavaScript needs `bigint` here.

## Pitfalls

- **A naive UTC time is read as local time.** Python `datetime.utcnow()` returns a naive `datetime`. Methods such as `timestamp()` treat a naive value as local time, so the result is wrong by the UTC offset of the system. Use `datetime.now(timezone.utc)`:

```python wrong id=python-naive-utc
import os
import time
from datetime import datetime, timezone

os.environ["TZ"] = "America/New_York"
time.tzset()

naive = datetime(2024, 1, 1, 12, 0)
aware = datetime(2024, 1, 1, 12, 0, tzinfo=timezone.utc)
print("naive:", naive.timestamp())
print("aware:", aware.timestamp())
```

```output for=python-naive-utc
naive: 1704128400.0
aware: 1704110400.0
```

- **The getters of `Date` use the local zone.** In JavaScript, `getFullYear`, `getHours` and `toString` use the time zone of the system, and `getUTCFullYear`, `getUTCHours` and `toISOString` use UTC. In New York, the epoch is in 1969:

```javascript wrong id=js-local-getters
process.env.TZ = "America/New_York";
const epoch = new Date(0);
console.log(epoch.getFullYear(), epoch.getUTCFullYear());
console.log(epoch.toISOString());
```

```output for=js-local-getters
1969 1970
1970-01-01T00:00:00.000Z
```

- **`Instant.toString` changes its length.** Java writes no fraction for a whole second, and three, six or nine digits for other values. Text of different lengths does not sort in time order, because `Z` sorts after `.`:

```java wrong id=java-instant-tostring
import java.time.Instant;

public class Main {
    public static void main(String[] args) {
        Instant a = Instant.ofEpochSecond(1_710_054_000L);
        Instant b = Instant.ofEpochSecond(1_710_054_000L, 120_000_000L);
        Instant c = Instant.ofEpochSecond(1_710_054_000L, 123_456_000L);
        System.out.println(a + " " + b + " " + c);
        System.out.println("a before b: " + a.isBefore(b));
        System.out.println("text a before text b: " + (a.toString().compareTo(b.toString()) < 0));
    }
}
```

```output for=java-instant-tostring
2024-03-10T07:00:00Z 2024-03-10T07:00:00.120Z 2024-03-10T07:00:00.123456Z
a before b: true
text a before text b: false
```

- **The default format of .NET comes from the culture.** `ToString()` with no arguments uses the culture of the thread, so the same value gives different text on different systems. Give a format and `CultureInfo.InvariantCulture`:

```csharp wrong id=csharp-culture
using System.Globalization;

var t = new DateTimeOffset(2024, 3, 10, 7, 0, 0, TimeSpan.Zero);
Console.WriteLine(t.ToString(CultureInfo.InvariantCulture));
Console.WriteLine(t.ToString(new CultureInfo("de-DE")));
Console.WriteLine(t.ToString("yyyy'-'MM'-'dd'T'HH':'mm':'ss'.'fff'Z'", CultureInfo.InvariantCulture));
```

```output for=csharp-culture
03/10/2024 07:00:00 +00:00
10.03.2024 07:00:00 +00:00
2024-03-10T07:00:00.000Z
```

- **`==` on a Go `Time` compares more than the instant.** A `Time` from `time.Now()` has a monotonic reading, and `Round(0)` removes it. `==` compares the readings and the location, so it says false for the same instant. Use `Equal`:

```go wrong id=go-time-equal
package main

import (
	"fmt"
	"time"
)

func main() {
	t := time.Now()
	u := t.Round(0)
	fmt.Println("t == u:", t == u)
	fmt.Println("t.Equal(u):", t.Equal(u))
	fmt.Println("UTC == local:", t.UTC() == t.Local())
}
```

```output for=go-time-equal
t == u: false
t.Equal(u): true
UTC == local: false
```

- **`gmtime` and `localtime` return one shared object.** In C, each call can overwrite the `struct tm` of the call before. Use `gmtime_r` and `localtime_r`, which fill a `struct tm` of the caller:

```c wrong id=c-gmtime-static
#include <stdio.h>
#include <time.h>

int main(void) {
    time_t a = 0;
    time_t b = 86400;
    struct tm *first = gmtime(&a);
    struct tm *second = gmtime(&b);
    printf("same object: %s\n", first == second ? "true" : "false");
    printf("day of the first: %d\n", first->tm_mday);
}
```

```output for=c-gmtime-static
same object: true
day of the first: 2
```

- **`printf '%(...)T' -1` is not 1969.** In the Bash builtin, the argument -1 means the current time and -2 means the time when the shell started. GNU `date -d @-1` gives the second before the epoch:

```bash wrong id=bash-printf-minus-one
export TZ=UTC0
printf '%(%Y-%m-%dT%H:%M:%S)T\n' 0
if [[ $(printf '%(%Y)T' -1) == 1969 ]]; then echo "printf -1: 1969"; else echo "printf -1: now, not 1969"; fi
date -u -d @-1 +%Y-%m-%dT%H:%M:%S
```

```output for=bash-printf-minus-one
1970-01-01T00:00:00
printf -1: now, not 1969
1969-12-31T23:59:59
```

- **A wall clock can go back.** The system clock is the time of day. A person or NTP can set it forward or back. Two readings do not give a correct duration, and a later reading can be lower. Use a monotonic clock to measure a duration (see [[measure-elapsed-time]]).

## Related tasks

- [[parse-and-format-a-date]]: write the instant in other formats, and read it back.
- [[measure-elapsed-time]]: measure a duration with a monotonic clock, not with the wall clock.
- [[get-a-unix-timestamp]]: the reading as seconds or milliseconds since the epoch.
- [[convert-between-time-zones]]: show the instant in the time zone of a person.

## References
