---
tag: NEW-calendar-arithmetic
kind: concept
slug: calendar-arithmetic
name: Calendar arithmetic
aliases: [date arithmetic, date math, add months, add days, end of month, period, duration, calendar duration, exact duration, wall-clock arithmetic, month overflow, clamp to end of month]
area: data-and-values/dates-and-times
level: core
summary: >-
  Calendar arithmetic adds days, months and years to dates. These units have no fixed length, so a library needs rules for the end of the month and for the days when the clocks change.
applies_to: core-languages
viz: optional
formal: none
see_also: [date-and-time-values, time-zone, iso-8601, add-days-to-a-date, compute-the-difference-of-two-dates, unix-time]
sources:
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/Period.html
  title: Class Period, Java SE 25 API
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/LocalDate.html
  title: Class LocalDate, Java SE 25 API
  locator: plusMonths; plusYears; plus(long, TemporalUnit)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.datetime.addmonths
  title: DateTime.AddMonths(Int32) Method, .NET API
  locator: Remarks
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Temporal/PlainDate/add
  title: Temporal.PlainDate.prototype.add(), MDN Web Docs
  locator: Parameters, overflow
- url: https://pkg.go.dev/time
  title: Package time, Go standard library
  locator: func (Time) AddDate
- url: https://en.cppreference.com/w/cpp/chrono/year_month_day/operator_arith
  title: std::chrono::year_month_day::operator+=, operator-=, cppreference.com
  locator: Example
- url: https://www.postgresql.org/docs/current/functions-datetime.html
  title: Date/Time Functions and Operators, PostgreSQL Documentation
  locator: 9.9 Date/Time Functions and Operators; Table 9.33 date_add
- url: https://www.rfc-editor.org/rfc/rfc5545.html
  title: "B. Desruisseaux, RFC 5545: Internet Calendaring and Scheduling Core Object Specification (iCalendar), September 2009"
  locator: 3.3.6 Duration
- url: https://docs.python.org/3/library/datetime.html
  title: datetime, Basic date and time types, The Python Standard Library
  locator: datetime objects, supported operations
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Temporal/Duration
  title: Temporal.Duration, MDN Web Docs
  locator: Calendar durations
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/mktime.html
  title: mktime, The Open Group Base Specifications Issue 8 (POSIX.1-2024)
  locator: Description
---

## Idea

A clock counts seconds, and a second always has the same length. A calendar counts days, months and years, and these units do not. A month has 28 to 31 days, a year has 365 or 366 days, and in a zone with daylight saving time a day has 23, 24 or 25 hours. So "one month after January 31" and "one day after 18:00" need rules, and the rules differ between libraries.

There are two kinds of amount. A duration is an exact length of time, such as 86400 seconds. A period is a count of calendar units, such as 1 month and 1 day. Java states the difference: "A Duration will add an exact number of seconds, thus a duration of one day is always exactly 24 hours. By contrast, a Period will add a conceptual day, trying to maintain the local time."[^1] The run block adds months with clamping and adds one day in two ways across a change of the clocks in New York.

```python run id=idea
import calendar
from datetime import date, datetime, timedelta, timezone
from zoneinfo import ZoneInfo

def add_months(d, n):
    # Move the month, then clamp the day to the length of the new month.
    y, m = divmod(d.month - 1 + n, 12)
    year, month = d.year + y, m + 1
    last = calendar.monthrange(year, month)[1]
    return d.replace(year=year, month=month, day=min(d.day, last))

jan31 = date(2024, 1, 31)
print(add_months(jan31, 1), add_months(jan31, 2), add_months(add_months(jan31, 1), 1))
print(add_months(date(2024, 2, 29), 12))
print((date(2024, 3, 1) - date(2024, 2, 1)).days, (date(2024, 4, 1) - date(2024, 3, 1)).days)

# One calendar day and 24 hours are different across a change of the clocks.
ny = ZoneInfo("America/New_York")
t = datetime(2024, 3, 9, 18, 0, tzinfo=ny)
print(t + timedelta(days=1))
print((t.astimezone(timezone.utc) + timedelta(hours=24)).astimezone(ny))
```

```output for=idea
2024-02-29 2024-03-31 2024-03-29
2025-02-28
29 31
2024-03-10 18:00:00-04:00
2024-03-10 19:00:00-04:00
```

## How it works

**Adding months and years.** A library adds the months to the month field and the years to the year field. Then it checks the day. Java's `plusMonths` "adds the specified amount to the months field in three steps"[^2]: it adds the months, it checks if the date is invalid, and it moves the day to the last valid day if necessary. Its example is 2007-03-31 plus one month, which would be the invalid date 2007-04-31: "Instead of returning an invalid result, the last valid day of the month, 2007-04-30, is selected instead."[^2] If the day is valid, it does not change.

**Three rules for an invalid day.** Libraries use one of three rules when the new month is too short:

- Clamp to the last day of the month. .NET states: "For example, March 31st + 1 month = April 30th, and March 31st - 1 month = February 28 for a non-leap year and February 29 for a leap year."[^3] Java, PostgreSQL and Temporal also clamp. In Temporal the option `overflow` controls it, and the default is to clamp: "The date component is clamped to the valid range."[^4]
- Overflow into the next month. Go normalizes the result, "so, for example, adding one month to October 31 yields December 1, the normalized form for November 31."[^5] JavaScript `Date`, C `mktime` and GNU `date` do the same.
- Report the error. With `overflow: 'reject'`, Temporal throws: "A RangeError is thrown if the date component is out of range."[^4] In C++, `year_month_day` plus `months` gives an invalid date such as 2024-02-31, and the program must check `ok()`[^6].

**Order matters.** Clamping loses information, so month arithmetic does not behave like the addition of numbers. In PostgreSQL, "March 31 plus 1 month becomes April 30, but March 31 plus 2 months becomes May 31."[^7] Thus adding 1 month two times can give a different result from adding 2 months, as the run block shows, and subtracting the month again does not always return the first date. A period with several fields is added from the largest field to the smallest: PostgreSQL adds the months, then the days, then the time, and "In both these steps the local time of day is kept the same."[^7]

**Days in a zone.** A calendar day in a zone with daylight saving time is not always 24 hours. Java gives the example of 18:00 on the evening before a gap: "The Period will add the conceptual day and result in a ZonedDateTime at 18:00 the following day. By contrast, the Duration will add exactly 24 hours, resulting in a ZonedDateTime at 19:00 the following day (assuming a one hour DST gap)."[^1] iCalendar states the same rule for its durations: "The duration of a week or a day depends on its position in the calendar."[^8] PostgreSQL shows it with `interval '1 day'` and `interval '24 hours'` on 2005-04-02 in Denver, and the results differ by one hour. "This happens because an hour was skipped due to a change in daylight saving time at 2005-04-03 02:00:00 in time zone America/Denver"[^7].

**Gaps and overlaps.** A calendar day added to a local time can give a local time that does not exist, or one that exists two times. For example, 02:30 on 2024-03-09 plus 1 day is 02:30 on 2024-03-10, which does not exist in New York. The library then uses its rule for gaps, such as a move forward by the length of the gap. See [[time-zone]].

**Differences.** A difference can also be exact or in calendar fields. Java's `Period.between` gives 2024-01-31 to 2024-03-01 as `P1M1D`, but the exact difference is 30 days. Python subtracts two aware values with the same zone as wall-clock times: "If both are naive, or both are aware and have the same tzinfo attribute, the tzinfo attributes are ignored"[^9]. Across the change of the clocks in November, that gives 1 day where 25 hours passed. See [[compute-the-difference-of-two-dates]].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `timedelta` for days and time; no months; `calendar.monthrange` to clamp by hand |
| JavaScript | native | `Date` `setMonth` and `setDate` overflow; `Temporal` `add` with the option `overflow` |
| TypeScript | native | the JavaScript `Date` and `Temporal` |
| Java | stdlib | `plusMonths` and `plusYears` clamp; `Period` and `Duration`; `Period.between` |
| Kotlin | stdlib | `java.time` on the JVM; `DatePeriod` and `DateTimePeriod` of kotlinx-datetime |
| C# | stdlib | `AddMonths` and `AddYears` clamp; `AddDays` on a `DateTime` with no zone |
| Go | stdlib | `AddDate` normalizes; `Add` for an exact `Duration` |
| Rust | library | no calendar in the standard library; the crates chrono and jiff |
| C | stdlib | add to the fields of `struct tm`; `mktime` normalizes them |
| C++ | stdlib | `year_month_day` plus `months` can be invalid; `local_days` and `sys_days` |
| Bash | idiom | GNU `date -d` with relative items such as `1 month` and `24 hours` |
| PowerShell | stdlib | the .NET `AddMonths`, `AddDays` and `AddHours` |
| SQL | native | `date` and `interval` arithmetic; PostgreSQL clamps months and keeps the local time |
| Regex | n/a | no dates and no arithmetic |
| HTML | n/a | no date arithmetic |
| CSS | n/a | no dates |

The modern libraries keep the two kinds of amount apart. Java has `Period` and `Duration`, Temporal has one `Duration` type but treats the calendar units in a special way, and C++ has separate types for calendar dates (`year_month_day`) and for time points. Temporal explains why a duration with months needs a date: "a calendar duration is not portable because the number of days in a month or year depends on the calendar system and the reference time point."[^10]

The older APIs normalize the fields. POSIX lets a program put any values in the fields of `struct tm`, and `mktime` uses them: "the values of these members shall not be restricted to the ranges described in"[^11] the header. So a program adds 1 to `tm_mon` or `tm_mday` and calls `mktime`, which moves February 31 to March 2 or March 3. JavaScript's `Date` and Go's `AddDate` work in the same way.

Python's standard library has no month arithmetic at all. A `timedelta` has days, seconds and microseconds. The Rust standard library has no calendar, so the crates chrono and jiff add months and days.

The example files add 1 month to January 31 and add 1 day to 18:00 before the spring change of the clocks in New York, in each language.

## Variations and related ideas

- **Date-only types.** A local date, such as Java's `LocalDate` or C++'s `year_month_day`, has no time of day and no zone, so adding days to it is never affected by daylight saving time. Use a date-only type when the time of day does not matter. See [[date-and-time-values]].
- **Durations in ISO 8601.** The text form `P1M1D` is a period and `PT24H` is a duration of 24 hours. See [[iso-8601]].
- **Recurring events.** A rule such as "on the 31st of every month" needs a decision for short months. iCalendar recurrence rules let a negative day count from the end of the month, so -1 means the last day.
- **Business days.** Adding working days skips weekends and holidays, and the list of holidays is data for each country.
- **Other calendars.** In a non-Gregorian calendar, months and years have other lengths. Temporal can do calendar arithmetic in other calendar systems.
- **Adding days in practice.** See [[add-days-to-a-date]].

## Pitfalls

- **86400 seconds as a day.** In a zone with daylight saving time, a day has 23 or 25 hours two times a year. Adding 86400 seconds to 18:00 gives 19:00 or 17:00 on those days. Add calendar days to a local time, or keep the value in UTC if the local time does not matter.
- **30 days as a month.** A month is 28 to 31 days. Adding 30 days to January 31 gives March 1 in a leap year. Use month arithmetic.
- **Month arithmetic that cannot be undone.** January 31 plus 1 month is February 29 in 2024, and February 29 minus 1 month is January 29. Store the first date and the number of months, and compute each date from the first date, not from the date before it.
- **February 29 plus 1 year.** Java and .NET give February 28, and Go and C give March 1. A program that stores birthdays or anniversaries must choose one rule.
- **Overflow where clamping was expected.** Go, JavaScript and C move January 31 plus 1 month to March 2 or March 3. Code that expects the end of February must clamp by hand, as the Go and JavaScript examples do.
- **Wall-clock subtraction.** Python subtracts two values with the same `tzinfo` as wall-clock times, and .NET `DateTime` has no zone. Convert both values to UTC before you compute elapsed time.
- **Text that changes the meaning.** In GNU `date`, a sign after a time of day is read as a UTC offset, so `18:00 +1 day` is wrong by several hours. The Bash example shows it.

## Cost

Calendar arithmetic on a date is a few integer operations: the month and year fields change, and a table of month lengths gives the clamp. A conversion between a date and a day count, which some libraries use for adding days, is also a short computation with no loops. Arithmetic on a zoned time costs more, because the library must find the offset before and after the operation in the rules of the zone. Exact arithmetic on an instant, such as adding a duration to a Unix time, is one addition.

## History

The C function `mktime` gave programs a simple method: put any values in the fields of `struct tm`, and the function normalizes them. JavaScript's `Date` and later Go's `AddDate` used the same method. The month overflow of these APIs is the result.

Libraries then separated the calendar units from exact time. The ISO 8601 durations had the forms `P1M` and `PT24H`. iCalendar, in RFC 5545 of 2009, defined that a day in a duration depends on its position in the calendar[^8]. Java SE 8, from the JSR 310 project, added `Period` and `Duration` as two classes[^1]. C++20 added `year_month_day` with an explicit check for invalid dates[^6]. JavaScript's `Temporal` added the option `overflow` for the choice between clamping and an error[^4].

## References
