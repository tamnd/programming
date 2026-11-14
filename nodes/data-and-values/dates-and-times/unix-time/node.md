---
tag: VKCWQ7
kind: concept
slug: unix-time
name: Unix time
aliases: [POSIX time, epoch time, Unix timestamp, epoch seconds, seconds since the epoch, time_t, the Unix epoch, year 2038 problem, Y2038]
area: data-and-values/dates-and-times
level: core
summary: >-
  Unix time is the number of seconds since 1970-01-01 00:00:00 UTC, with each day counted as exactly 86400 seconds. Most systems store and send instants in this form.
applies_to: core-languages
viz: optional
formal: none
see_also: [date-and-time-values, measure-elapsed-time, integer-overflow, integer-representation, parse-and-format-a-date, get-the-modification-time-of-a-file]
sources:
- url: https://en.wikipedia.org/wiki/Unix_time
  title: Unix time, Wikipedia
  locator: Definition; Leap seconds; Representing the number; History; Range of representable times
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap04.html
  title: POSIX.1-2024, Base Definitions, General Concepts
  locator: 4.19 Seconds Since the Epoch
- url: https://docs.python.org/3/library/time.html
  title: time, Time access and conversions, The Python Standard Library
  locator: time.time; time.time_ns
- url: https://en.wikipedia.org/wiki/Leap_second
  title: Leap second, Wikipedia
  locator: History; Leap second smearing
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/Instant.html
  title: Class Instant, Java SE 25 API
  locator: Description; Time-scale
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/now
  title: Date.now(), MDN Web Docs
  locator: Description; Reduced time precision
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.html
  title: Class System, Java SE 25 API
  locator: currentTimeMillis()
- url: https://en.cppreference.com/w/c/chrono/time
  title: time, cppreference.com
  locator: Notes
- url: https://pkg.go.dev/time
  title: Package time, Go standard library
  locator: func (Time) Unix; func (Time) UnixNano
- url: https://learn.microsoft.com/en-us/dotnet/api/system.datetimeoffset.tounixtimeseconds
  title: DateTimeOffset.ToUnixTimeSeconds Method, .NET API
  locator: Remarks
- url: https://en.cppreference.com/w/cpp/chrono/system_clock
  title: std::chrono::system_clock, cppreference.com
  locator: Description
- url: https://doc.rust-lang.org/std/time/struct.SystemTime.html
  title: Struct SystemTime, The Rust Standard Library
  locator: Description; UNIX_EPOCH
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 5.2 Bash Variables, EPOCHSECONDS
- url: https://en.wikipedia.org/wiki/Year_2038_problem
  title: Year 2038 problem, Wikipedia
  locator: Possible solutions; Implemented solutions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date
  title: Date, MDN Web Docs
  locator: The epoch, timestamps, and invalid date
---

## Idea

Unix time gives each instant one number. The number is the count of seconds after a fixed instant, the Unix epoch: 1970-01-01 00:00:00 UTC. In the words of the encyclopedia: "It measures time by the number of non-leap seconds that have elapsed since 00:00:00 UTC on 1 January 1970, the Unix epoch."[^1] Thus 0 is the epoch, 1709985600 is 2024-03-09 12:00:00 UTC, and a negative number is an instant before 1970.

The count has one rule that makes it simple: POSIX says "As represented in seconds since the Epoch, each and every day shall be accounted for by exactly 86400 seconds."[^2] Because of this rule, a program can get the day and the time of day with one division, and it can get the number back from calendar fields with a fixed expression.

```python run id=idea
from datetime import datetime, timezone

t = 1709985600
# Each day is 86400 seconds: days since the epoch and the second of the day.
print(divmod(t, 86400))
print(datetime.fromtimestamp(t, tz=timezone.utc))

# The POSIX expression from the fields year - 1900, day of the year (from 0),
# hour, minute and second.
def posix_seconds(tm_year, tm_yday, hour, minute, sec):
    return (sec + minute * 60 + hour * 3600 + tm_yday * 86400
            + (tm_year - 70) * 31536000 + ((tm_year - 69) // 4) * 86400
            - ((tm_year - 1) // 100) * 86400 + ((tm_year + 299) // 400) * 86400)

print(posix_seconds(124, 68, 12, 0, 0))

# Many APIs count milliseconds instead of seconds.
print(t * 1000)
```

```output for=idea
(19791, 43200)
2024-03-09 12:00:00+00:00
1709985600
1709985600000
```

The day 2024-03-09 is day 19791 after the epoch, and 43200 seconds is 12:00. The expression gives the same number from the fields. A Unix time has no time zone: it names an instant, and a program picks a zone only to show it.

## How it works

**The count.** The number goes up by 1 for each second. Python defines the epoch as "the point where the time starts, the return value of time.gmtime(0). It is January 1, 1970, 00:00:00 (UTC) on all platforms."[^3] The value is a plain integer, so a program can store, compare, sort and subtract it with no time library. When no leap second falls between two instants, "the difference between two Unix time numbers is equal to the duration in seconds of the period between the corresponding points in time."[^1]

**From fields to a number.** POSIX defines the number for a UTC date and time with an expression in C. It adds the seconds, minutes, hours and days of the year, 365 days for each year after 1970, and a day for each leap year. The last three terms of the expression are the Gregorian leap-year rule: a day every 4 years, minus a day every 100 years, plus a day every 400 years[^2]. The function `posix_seconds` in the Idea section is this expression. The reverse is a division by 86400, then a conversion of the day count to a date. See [[calendar-arithmetic]].

**Leap seconds.** UTC has had 27 leap seconds since 1972[^4], and a UTC day with a leap second has 86401 seconds. Unix time does not count them. POSIX says that the relation of its count to the actual UTC time "is unspecified"[^2]. On a conforming system, "Unix time numbers are repeated in the second immediately following a positive leap second."[^1] Thus across a leap second, "such calculations give the wrong answer."[^1] Java names the same idea the Java Time-Scale: "The Java Time-Scale divides each calendar day into exactly 86400 subdivisions, known as seconds. These seconds may differ from the SI second."[^5] See [[leap-seconds]].

**The unit.** The original unit is the second, but many APIs count a smaller unit from the same epoch. JavaScript counts milliseconds: "The Date.now() static method returns the number of milliseconds elapsed since the epoch, which is defined as the midnight at the beginning of January 1, 1970, UTC."[^6] Java `System.currentTimeMillis` also "Returns the current time in milliseconds."[^7] Go and Python also give nanoseconds. The number does not show its unit, so a program must know it from the API or the name of the field.

**The width.** The C type `time_t` holds the count. C does not fix its form: "The encoding of calendar time in time_t is unspecified, but most systems conform to POSIX specification and return a value of integral type holding the number of seconds since the Epoch."[^8] A signed 32-bit `time_t` ends in 2038. A 64-bit one lasts far beyond any date that a program needs.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `time.time()` (a float) and `time.time_ns()`; `datetime.fromtimestamp()`, `datetime.timestamp()` and `calendar.timegm()` |
| JavaScript | native | `Date.now()` and `getTime()` in milliseconds; `new Date(ms)`; `Temporal.Instant` with `epochMilliseconds` and `epochNanoseconds` |
| TypeScript | native | the JavaScript `Date` and `Temporal` |
| Java | stdlib | `Instant.ofEpochSecond()`, `getEpochSecond()` and `toEpochMilli()`; `System.currentTimeMillis()` |
| Kotlin | stdlib | `java.time.Instant` on the JVM; `Instant` of kotlinx-datetime on all platforms |
| C# | stdlib | `DateTimeOffset.FromUnixTimeSeconds()`, `ToUnixTimeSeconds()` and `ToUnixTimeMilliseconds()`; `DateTime.UnixEpoch` |
| Go | stdlib | `time.Unix()`, `time.UnixMilli()`; the methods `Unix()`, `UnixMilli()` and `UnixNano()` |
| Rust | stdlib | `SystemTime::now()`, `UNIX_EPOCH` and `duration_since()` |
| C | stdlib | `time()` and `time_t`; `timespec_get()`; `gmtime()` for the fields |
| C++ | stdlib | `std::chrono::system_clock`, `sys_seconds` and `sys_days`; `to_time_t()` |
| Bash | idiom | `EPOCHSECONDS`, `EPOCHREALTIME` and the format `%(datefmt)T` of `printf`; the external command `date +%s` |
| PowerShell | stdlib | `[DateTimeOffset]::UtcNow.ToUnixTimeSeconds()`; `Get-Date -UFormat %s` |
| SQL | idiom | no standard function; each system has its own, such as `extract(epoch from ...)` and `to_timestamp()` in PostgreSQL, `UNIX_TIMESTAMP()` in MySQL and `unixepoch()` in SQLite |
| Regex | n/a | no time values |
| HTML | n/a | dates in HTML are text in ISO 8601 form, not numbers |
| CSS | n/a | no dates |

Most languages have a direct conversion between their instant type and a Unix time. In Java, "The epoch-seconds are measured from the standard Java epoch of 1970-01-01T00:00:00Z where instants after the epoch have positive values, and earlier instants have negative values."[^5] In Go, "Unix returns t as a Unix time, the number of seconds elapsed since January 1, 1970 UTC."[^9] In .NET, "Unix time represents the number of seconds that have elapsed since 1970-01-01T00:00:00Z (January 1, 1970, at 12:00 AM UTC). It does not take leap seconds into account."[^10]

The C++ `system_clock` "measures Unix Time (i.e., time since 00:00:00 Coordinated Universal Time (UTC), Thursday, 1 January 1970, not counting leap seconds)."[^11] Rust keeps the epoch as a constant, `UNIX_EPOCH`, which is "An anchor in time which can be used to create new SystemTime instances or learn about where in time a SystemTime lies."[^12] In Bash, `EPOCHSECONDS` is a variable: "Each time this parameter is referenced, it expands to the number of seconds since the Unix Epoch"[^13].

The example files convert 1709985600 to a date and time in UTC and back in each language. They also show milliseconds, a value before 1970 and the end of a signed 32-bit count. The C example computes the POSIX expression from a `struct tm`.

## Variations and related ideas

- **Other units.** Milliseconds (JavaScript, Java), microseconds (`EPOCHREALTIME` in Bash) and nanoseconds (`time_ns` in Python, `UnixNano` in Go) count from the same epoch. Python says: "Use time_ns() to avoid the precision loss caused by the float type."[^3]
- **Other epochs.** .NET `DateTime` counts ticks of 100 nanoseconds from 0001-01-01, Windows `FILETIME` counts from 1601, and the Network Time Protocol counts from 1900[^1]. The C# example prints the tick count of the Unix epoch.
- **Counts with leap seconds.** Some systems count every second, which makes the count a form of TAI. This variant is "much rarer"[^1], and "some Linux systems are configured this way."[^1] See [[leap-seconds]].
- **Smeared leap seconds.** Some services spread a leap second over many hours, so each second of the count is a bit longer and the count never repeats[^4]. See [[leap-seconds]].
- **Text forms.** A timestamp in an API is often text in the ISO 8601 form `2024-03-09T12:00:00Z` instead of a number. The text shows its unit and its zone, but it is longer. See [[iso-8601]].

## Pitfalls

- **Seconds where milliseconds are expected.** A Unix time in seconds given to an API that expects milliseconds gives a date in January 1970. The other way, it gives a date thousands of years in the future. The JavaScript example shows `new Date(1709985600)`, which is 1970-01-20. Put the unit in the name of the field, such as `created_at_ms`.
- **The year 2038.** A signed 32-bit count ends at 2038-01-19 03:14:07 UTC: "One second after 2038-01-19T03:14:07Z this representation will overflow in what is known as the year 2038 problem"[^1]. After that second, the value wraps to 1901, as the Java, Go and C examples show. A 64-bit `time_t` does not help when a program stores the value in a 32-bit field of a file format, a protocol or a database column.
- **Leap seconds in differences.** The difference of two Unix times is not the true number of elapsed seconds when a leap second falls between them. For exact intervals, use a monotonic clock or a time scale such as TAI.
- **Floats.** A float of seconds loses precision. A 64-bit float has 53 bits of mantissa, so a current Unix time in a float keeps only about a quarter of a microsecond. Python `time.time()` returns a float, and Python warns about this. Use integers of a fixed unit.
- **Values before 1970.** POSIX defines the expression only for later times: "If the year is <1970 or the value is negative, the relationship is undefined."[^2] Most libraries accept negative values, but some functions and platforms reject them. In Rust, `duration_since(UNIX_EPOCH)` returns an error for an earlier time.
- **The wall clock is not monotonic.** The current Unix time comes from the system clock, which can move back. For Rust `SystemTime`, "Distinct from the Instant type, this time measurement is not monotonic"[^12]. Do not measure durations with it. See [[measure-elapsed-time]].
- **Local time added twice.** A Unix time is already in UTC. Code that adds the offset of the local zone to it, and then formats it as local time, shows the offset two times. In Go, the value of `Unix()` does not depend on the zone: "The result does not depend on the location associated with t."[^9]

## Cost

| Representation | Range | Resolution |
|---|---|---|
| signed 32-bit seconds | from 1901-12-13 to 2038-01-19[^1] | 1 second |
| unsigned 32-bit seconds | from 1970 to 2106-02-07[^14] | 1 second |
| signed 64-bit seconds | about 292 billion years in both directions[^1] | 1 second |
| JavaScript `Date` (milliseconds in a float) | from April 20, 271821 BC to September 13, 275760 AD[^15] | 1 millisecond |
| signed 64-bit nanoseconds (Go `UnixNano`) | from 1678 to 2262[^9] | 1 nanosecond |
| 64-bit float of seconds (Python `time.time()`) | wide | about 0.24 microseconds in 2024 |

A Unix time costs one machine word. Comparison, sorting and subtraction are single integer operations, and a database can index the value as a plain integer. The conversion to calendar fields needs a division and a short computation for the year, the month and the day, but no table, because the count has no leap seconds and no zones. Go states the limit of its nanosecond form: "The result is undefined if the Unix time in nanoseconds cannot be represented by an int64 (a date before the year 1678 or after 2262)."[^9] The precision of the current time also depends on the system. For Java, "the granularity of the value depends on the underlying operating system and may be larger."[^7] Browsers can reduce the precision of `Date.now()` on purpose, "To offer protection against timing attacks and fingerprinting"[^6].

## History

The first edition of the Unix Programmer's Manual, from 3 November 1971, says: "time returns the time since 00:00:00, Jan. 1, 1971, measured in sixtieths of a second."[^1] A 32-bit count of sixtieths lasts only about two years. The third edition of 15 March 1972 moved the epoch to 1972 and said: "This guarantees a crisis every 2.26 years."[^1] Later versions counted whole seconds from 1970. In the words of the encyclopedia: "The current epoch of 1 January 1970 00:00:00 UTC was selected arbitrarily by Unix engineers because it was considered a convenient date to work with."[^1]

POSIX.1 fixed the definition of seconds since the Epoch, with the expression from fields and the rule of exactly 86400 seconds for each day[^2]. It did not count leap seconds, so the count stays simple and is not exact across a leap second.

The year 2038 problem then became the main issue. An unsigned 32-bit type would last until 2106, but it "would adversely affect programs that store, retrieve, or manipulate dates prior to 1970"[^14]. So systems moved to a signed 64-bit `time_t`. NetBSD 6.0 in October 2012 and OpenBSD 5.5 in May 2014 use a 64-bit `time_t` on 32-bit and 64-bit platforms[^14]. On Linux, "Starting with version 5.6 of 2020, 64-bit time_t is supported on 32-bit architectures, too."[^14] The GNU C Library 2.34 of August 2021 lets a program choose a 64-bit `time_t` on 32-bit platforms with the macro `_TIME_BITS`[^14]. C++20 defined `system_clock` as Unix time, which before was only the common practice[^11].

## References
