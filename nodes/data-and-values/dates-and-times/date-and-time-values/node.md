---
tag: 4W6JX1
kind: concept
slug: date-and-time-values
name: Date and time values
aliases: [date and time types, date-time, datetime, instant, timestamp, local date, civil time, duration, period, naive and aware datetimes]
area: data-and-values/dates-and-times
level: core
summary: >-
  A program keeps time in different values: an instant on the global time-line, a local date and time as people read it, and amounts of time as exact durations or calendar periods.
applies_to: core-languages
viz: optional
formal: none
see_also: [measure-elapsed-time, parse-and-format-a-date, immutability, integer-overflow, get-the-modification-time-of-a-file]
sources:
- url: https://docs.python.org/3/library/datetime.html
  title: datetime, Basic date and time types, The Python Standard Library
  locator: Aware and naive objects; Constants; Available types
- url: https://en.cppreference.com/w/cpp/chrono
  title: Date and time library, cppreference.com
  locator: Clocks; Time point; Calendar; Time zone
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap04.html
  title: POSIX.1-2024, Base Definitions, General Concepts
  locator: 4.19 Seconds Since the Epoch
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date
  title: Date, MDN Web Docs
  locator: Description; The epoch, timestamps, and invalid date
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/package-summary.html
  title: Package java.time, Java SE 25 API
  locator: Description; Dates and Times; Duration and Period
- url: https://pkg.go.dev/time
  title: Package time, Go standard library
  locator: Monotonic Clocks; type Time
- url: https://learn.microsoft.com/en-us/dotnet/standard/datetime/choosing-between-datetime
  title: Choose between DateTime, DateOnly, DateTimeOffset, TimeSpan, TimeOnly, and TimeZoneInfo, .NET
  locator: The DateTimeOffset structure
- url: https://doc.rust-lang.org/std/time/struct.Instant.html
  title: Struct Instant, The Rust Standard Library
  locator: Description
- url: https://doc.rust-lang.org/std/time/struct.SystemTime.html
  title: Struct SystemTime, The Rust Standard Library
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Temporal
  title: Temporal, MDN Web Docs
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/Date
  title: Date() constructor, MDN Web Docs
  locator: Parameters
- url: https://en.cppreference.com/w/c/chrono/tm
  title: tm, cppreference.com
  locator: Member objects
- url: https://en.wikipedia.org/wiki/Unix_time
  title: Unix time, Wikipedia
  locator: Range of representable times; History
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Date.html
  title: Class Date, Java SE 25 API
  locator: Description
- url: https://jcp.org/en/jsr/detail?id=310
  title: JSR 310, Date and Time API, Java Community Process
  locator: Description; Stage timeline
- url: https://docs.python.org/3/whatsnew/2.3.html
  title: What's New in Python 2.3
  locator: Introduction; Other New, Improved, and Deprecated Modules
- url: https://github.com/tc39/proposal-temporal
  title: Temporal proposal, TC39
  locator: Status
---

## Idea

A program meets time in two forms. The first form is a point on the global time-line, such as the moment when a payment went through or a log line was written. The second form is what people read on a calendar and a clock, such as 9 March 2024, 12:00. The two forms are not the same, because one moment has different calendar readings in different places. Python names the split with the words aware and naive: "An aware object represents a specific moment in time that is not open to interpretation."[^1] But "A naive object does not contain enough information to unambiguously locate itself relative to other date/time objects."[^1]

Amounts of time also have two forms. A duration, such as 90 minutes, is an exact number of seconds. A period, such as 1 month, is a number of calendar units, and its length in seconds depends on the date where it starts.

```python run id=idea
from datetime import date, datetime, timedelta, timezone

# An instant: a moment on the time-line, written here in UTC.
paid = datetime(2024, 3, 9, 12, 0, tzinfo=timezone.utc)
# The same instant on a clock in a zone that is 9 hours ahead of UTC.
print(paid.astimezone(timezone(timedelta(hours=9))))

# A local date and time: a calendar and clock reading with no zone.
meeting = datetime(2024, 3, 9, 12, 0)
print(meeting, meeting.tzinfo)

# A date with no time, and an exact duration.
print(date(2024, 3, 9), timedelta(minutes=90))

# The usual number for an instant: seconds since 1970-01-01 00:00 UTC.
print(int(paid.timestamp()))
```

```output for=idea
2024-03-09 21:00:00+09:00
2024-03-09 12:00:00 None
2024-03-09 1:30:00
1709985600
```

The value `paid` is one moment, and the program can show it on any clock. The value `meeting` has no zone, so it is not a moment until the program adds a zone. Most bugs with dates come from a mix of these kinds of values.

## How it works

**An instant is a count from an epoch.** Most systems keep an instant as a number of units after a fixed moment. In C++, "A clock consists of a starting point (or epoch) and a tick rate."[^2] and "A time point is a duration of time that has passed since the epoch of a specific clock."[^2] POSIX defines Seconds Since the Epoch as "A value that approximates the number of seconds that have elapsed since the Epoch."[^3] JavaScript `Date` objects "encapsulate an integral number that represents milliseconds since the midnight at the beginning of January 1, 1970, UTC"[^4]. In Java, "Instant is essentially a numeric timestamp."[^5] In Go, "A Time represents an instant in time with nanosecond precision."[^6] The unit is different in each system: seconds in POSIX, milliseconds in JavaScript and nanoseconds in Go. See [[unix-time]].

**A local date and time is a set of fields.** The fields are the year, month, day, hour, minute and second. In Java, "LocalDate stores a date without a time."[^5] Python describes its date type as "An idealized naive date, assuming the current Gregorian calendar always was, and always will be, in effect."[^1] Most libraries use this proleptic Gregorian calendar. Fields are good for display and for calendar steps, such as the same day in the next month.

**A zone joins the two forms.** To turn a local date and time into an instant, a program needs the offset from UTC at that date and place. A value with an offset names one instant. In .NET, "The DateTimeOffset structure represents a date and time value, together with an offset that indicates how much that value differs from UTC."[^7] A time zone such as `America/New_York` has rules that change its offset during the year. Thus a local time can be missing on the day when the clocks move forward, and it can occur two times on the day when they move back. See [[time-zone]] and [[tz-database]].

**Durations and periods.** Java has a type for each: "A Duration is a simple measure of time along the time-line in nanoseconds. A Period expresses an amount of time in units meaningful to humans, such as years or days."[^5] In New York, 2024-03-09 12:00 plus a period of 1 day is 2024-03-10 12:00, but plus a duration of 24 hours is 13:00, because the clocks moved forward in that night. The Java example shows this. See [[calendar-arithmetic]].

**Two clocks.** A computer has a wall clock, which the system can set back or forward, and a monotonic clock, which only moves forward. Go states the rule: "The general rule is that the wall clock is for telling time and the monotonic clock is for measuring time."[^6] Rust has a type for each clock. `Instant` is "A measurement of a monotonically nondecreasing clock."[^8] `SystemTime` is "A measurement of the system clock, useful for talking to external entities like the file system or other processes."[^9] See [[measure-elapsed-time]].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `datetime` (aware or naive), `date`, `time` and `timedelta`; `zoneinfo` for zones |
| JavaScript | native | `Date`, an instant in milliseconds with a mutable API; `Temporal` with `Instant`, `PlainDate`, `ZonedDateTime` and `Duration` |
| TypeScript | native | the JavaScript `Date` and `Temporal` |
| Java | stdlib | `java.time`: `Instant`, `LocalDate`, `LocalDateTime`, `ZonedDateTime`, `Duration` and `Period`; the old `java.util.Date` |
| Kotlin | stdlib | `java.time` on the JVM; `kotlin.time.Duration`; the library kotlinx-datetime |
| C# | stdlib | `DateTime`, `DateTimeOffset`, `DateOnly`, `TimeOnly`, `TimeSpan` and `TimeZoneInfo` |
| Go | stdlib | `time.Time`, an instant with a location and a monotonic reading; `time.Duration` |
| Rust | stdlib | `SystemTime`, `Instant` and `Duration`, with no calendar types; the crates chrono, time and jiff |
| C | stdlib | `time_t`, `struct tm` and `struct timespec`; `gmtime`, `mktime` and `strftime` |
| C++ | stdlib | `std::chrono` clocks, `time_point` and `duration`; `year_month_day` and `zoned_time` |
| Bash | idiom | the variable `EPOCHSECONDS` and the format `%(datefmt)T` of `printf`; the external command `date` |
| PowerShell | stdlib | the .NET types; the cmdlets `Get-Date` and `New-TimeSpan` |
| SQL | native | the types `DATE`, `TIME`, `TIMESTAMP`, `TIMESTAMP WITH TIME ZONE` and `INTERVAL` |
| Regex | n/a | no time values; a pattern can check only the form of a date string |
| HTML | idiom | the `time` element with a machine-readable `datetime` attribute; date inputs |
| CSS | n/a | no dates; the `time` type, such as `200ms`, is a duration for animations |

JavaScript has two APIs. The old one is `Date`: "With the introduction of the Temporal API, the Date object is considered a legacy feature."[^4] The `Temporal` object "is designed as a full replacement for the Date object."[^10] It has separate types for an instant, a zoned date and time, a plain date and a duration, as Java has.

Java made the same change earlier. The package `java.time` has a type for each kind of value, and "All the classes are immutable and thread-safe."[^5] .NET added `DateTimeOffset` to `DateTime`, and "The DateTimeOffset type includes all of the functionality of the DateTime type along with time zone awareness."[^7] Go uses one type, `time.Time`, for an instant, a local reading and a date. C keeps an instant in a `time_t` and its fields in a `struct tm`. The Rust standard library has clocks and durations but no calendar.

The example files show these forms. Python compares an aware and a naive value. JavaScript shows a `Date` that changes, a field that rolls over and an invalid date. Java adds a period and a duration across the start of daylight saving time in New York. C# uses `DateTimeOffset`, `DateOnly` and `TimeSpan`. Go compares two `time.Time` values with `Equal` and with `==`. Rust uses `SystemTime`, `Instant` and `Duration`. C converts a `time_t` to a `struct tm` and back. C++ uses the calendar types of `std::chrono`.

## Variations and related ideas

- **Time scales.** UTC follows the rotation of the Earth with leap seconds, and other scales do not. C++ has a clock for each scale, such as a "Clock for International Atomic Time (TAI)"[^2] and a "Clock for GPS time"[^2]. See [[leap-seconds]].
- **Text forms.** A program sends and stores a time as text in the formats of ISO 8601 and RFC 3339, such as `2024-03-09T12:00:00Z`. See [[iso-8601]] and [[parse-and-format-a-date]].
- **Partial dates.** Some values have only some fields: a month and a day for a birthday, or a year and a month for a card expiry date. Java has `MonthDay`, `YearMonth`, `Month` and `DayOfWeek`. In Java, "Month stores a month on its own."[^5]
- **Other calendars.** Most libraries use the Gregorian calendar for computation and other calendars for display. Java has "the calendar neutral API"[^5] in the package `java.time.chrono`, and `Temporal` has a calendar in each date.
- **Elapsed time.** A monotonic clock gives durations but no dates. See [[measure-elapsed-time]].

## Pitfalls

- **Months that start at 0.** The JavaScript `Date` constructor takes a month index: an "Integer value representing the month, beginning with 0 for January to 11 for December."[^11] Thus `new Date(2024, 1, 1)` is 1 February. In C, the field `tm_mon` of `struct tm` counts the "months since January"[^12], and `tm_year` counts the "years since 1900"[^12].
- **A mix of naive and aware values.** A naive value has no zone, and "Whether a naive object represents Coordinated Universal Time (UTC), local time, or time in some other time zone is purely up to the program"[^1]. In Python, an order test between a naive and an aware value raises `TypeError`, and an equality test is false. Keep instants aware, and convert at the edges of the program.
- **Mutable date objects.** A JavaScript `Date` and a Java `java.util.Date` can change. A date in the field of an object changes when other code calls a setter on it. Copy the value, or use immutable types such as `java.time` and `Temporal`.
- **Field equality for instants.** Two values for one instant can have different fields. Go says: "In general, prefer t.Equal(u) to t == u, since t.Equal uses the most accurate comparison available and correctly handles the case when only one of its arguments has a monotonic clock reading."[^6] The Go example shows that `==` is false for one instant in two zones.
- **The wall clock for elapsed time.** The system clock can move back. For Rust `SystemTime`, "Distinct from the Instant type, this time measurement is not monotonic."[^9] Measure elapsed time with a monotonic clock.
- **The local zone of the machine.** A function that uses the local zone, such as C `localtime` or a JavaScript `Date` getter without `UTC` in its name, gives different results on different machines. Tests that pass on one machine can fail on a server in UTC. Give the zone explicitly.

## Cost

| Value | Range | Precision |
|---|---|---|
| 32-bit `time_t` | from 1901-12-13 to 2038-01-19[^13] | 1 second |
| 64-bit `time_t` | about 292.3 billion years in both directions[^13] | 1 second |
| JavaScript `Date` | from April 20, 271821 BC to September 13, 275760 AD[^4] | 1 millisecond |
| Python `datetime` | the years 1 to 9999[^1] | 1 microsecond |
| Go `time.Time`, Java `Instant` | far beyond the dates of most programs | 1 nanosecond |

An instant as one integer is cheap to store, compare and sort. A conversion to fields costs more, because it needs the calendar rules and, for a local time, the rules of the zone. A 32-bit count of seconds has a hard limit: "One second after 2038-01-19T03:14:07Z this representation will overflow in what is known as the year 2038 problem"[^13]. Python sets its limits in constants: "MINYEAR is 1."[^1] and "MAXYEAR is 9999."[^1]

## History

The first Unix systems counted time from a recent epoch. The first edition of the Unix Programmer's Manual, from 3 November 1971, says that "time returns the time since 00:00:00, Jan. 1, 1971, measured in sixtieths of a second."[^13] Later versions moved the epoch to 1970 and counted whole seconds, and C and POSIX took that count as `time_t`, with `struct tm` for the fields[^3].

The first libraries mixed the kinds of values. Java 1.0 put an instant and its fields into one class. Today the documentation says: "The class Date represents a specific instant in time, with millisecond precision. Prior to JDK 1.1, the class Date had two additional functions."[^14] Java later added `Calendar`, and then JSR 310, which says: "This JSR will provide a new and improved date and time API for Java."[^15] Its final release was on 4 March 2014[^15], as the package `java.time`.

Python added the module `datetime` in Python 2.3: "A set of standard date/time types has been added in the new datetime module."[^16] "Python 2.3 was released on July 29, 2003."[^16] C++ added `std::chrono` with clocks and durations in C++11, and calendar dates and time zones in C++20[^2]. In JavaScript, the `Temporal` proposal reached Stage 4 at TC39, and it shipped in Firefox 139 on 2025-05-27, in Chrome 144 on 2026-01-13 and in Node 26 on 2026-05-05[^17].

## References
