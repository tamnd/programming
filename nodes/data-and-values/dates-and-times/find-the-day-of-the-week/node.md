---
tag: 0Q43MZ
kind: task
slug: find-the-day-of-the-week
name: Find the day of the week
aliases: [day of the week, weekday, weekday of a date, ISO weekday, ISO week number, week of the year, week-based year, Zeller's congruence, getDay, DayOfWeek, isoweekday]
area: data-and-values/dates-and-times
level: intro
summary: Find the weekday of a calendar date and its ISO 8601 week. Libraries number the days from 0 or from 1, and from Sunday or from Monday, and the week-based year is not always the calendar year.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/datetime.html
    title: The Python Standard Library, datetime
    locator: date.weekday(); date.isoweekday(); date.isocalendar()
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getDay
    title: MDN Web Docs, Date.prototype.getDay()
    locator: Return value
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getUTCDay
    title: MDN Web Docs, Date.prototype.getUTCDay()
    locator: Return value
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/parse
    title: MDN Web Docs, Date.parse()
    locator: Examples
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/DayOfWeek.html
    title: Java SE 25 API, java.time.DayOfWeek
    locator: the description of the enum
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/temporal/IsoFields.html
    title: Java SE 25 API, java.time.temporal.IsoFields
    locator: WEEK_BASED_YEAR; WEEK_OF_WEEK_BASED_YEAR
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/format/DateTimeFormatter.html
    title: Java SE 25 API, java.time.format.DateTimeFormatter
    locator: Patterns for Formatting and Parsing
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.dayofweek?view=net-10.0
    title: .NET API reference, DayOfWeek Enum
    locator: Fields
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.globalization.isoweek?view=net-10.0
    title: .NET API reference, ISOWeek Class
    locator: Methods
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.globalization.calendar.getweekofyear?view=net-10.0
    title: .NET API reference, Calendar.GetWeekOfYear
    locator: Remarks
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: type Weekday; func (Time) Weekday; func (Time) ISOWeek
  - url: https://en.cppreference.com/w/cpp/chrono/weekday/encoding.html
    title: cppreference.com, std::chrono::weekday::c_encoding, std::chrono::weekday::iso_encoding
    locator: Return value
  - url: https://howardhinnant.github.io/date_algorithms.html
    title: Howard Hinnant, chrono-Compatible Low-Level Date Algorithms
    locator: days_from_civil; weekday_from_days
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/strftime.html
    title: The Open Group Base Specifications Issue 8, strftime
    locator: "DESCRIPTION: %u, %w, %G, %V"
  - url: https://man7.org/linux/man-pages/man1/date.1.html
    title: Linux manual page, date(1)
    locator: "FORMAT: %u, %G, %V"
  - url: https://en.wikipedia.org/wiki/ISO_week_date
    title: Wikipedia, ISO week date
    locator: First week
  - url: https://en.wikipedia.org/wiki/Zeller%27s_congruence
    title: Wikipedia, Zeller's congruence
    locator: Formula; Implementations in software
---

## Task

Find the day of the week of a calendar date. A date here has no time of day and no time zone, such as `2024-03-10`, which was a Sunday.

The contract `day_of_week(year, month, day)` returns the number of ISO 8601: 1 for Monday, 2 for Tuesday and so on to 7 for Sunday. The dates use the proleptic Gregorian calendar for the years 1 to 9999.

Libraries use different numbers for the same day:

| API | Monday | Sunday |
| --- | --- | --- |
| ISO 8601, Python `isoweekday()`, Java `DayOfWeek.getValue()`, C++ `iso_encoding()`, `date +%u` | 1 | 7 |
| Python `weekday()` | 0 | 6 |
| JavaScript `getDay()`, C `tm_wday`, C# `DayOfWeek`, Go `Weekday`, C++ `c_encoding()`, `date +%w` | 1 | 0 |

ISO 8601 also numbers the weeks. A week starts on Monday, and week 1 of a year is the week with the first Thursday of the year. So the days at the start and end of a calendar year can be in a week of the year before or the year after. 2024-12-30 is a Monday in week 1 of the week-based year 2025, and 2027-01-01 is a Friday in week 53 of 2026. Each solution also has an `iso_week(year, month, day)` function that returns the week-based year and the week number.

## Solutions

Each solution reads the date with no time zone, or as midnight UTC. A date read in local time can be the day before (see the pitfalls).

- **Python.** `date(year, month, day).isoweekday()` gives 1 to 7. `date.isocalendar()` gives the week-based year, the week and the weekday.
- **JavaScript.** The solution sets the date with `setUTCFullYear` and calls `getUTCDay()`, which returns 0 for Sunday. `getUTCDay() || 7` changes 0 to 7. There is no function for the ISO week, so the solution moves to the Thursday of the same week. The year of that Thursday is the week-based year, and its day of the year gives the week.
- **TypeScript.** The same code as JavaScript, with the type `number`.
- **Java.** `LocalDate.getDayOfWeek()` returns a `DayOfWeek`, and its documentation says that "The int value follows the ISO-8601 standard, from 1 (Monday) to 7 (Sunday)." `date.get(IsoFields.WEEK_BASED_YEAR)` and `date.get(IsoFields.WEEK_OF_WEEK_BASED_YEAR)` give the ISO week.
- **C#.** `DateOnly.DayOfWeek` is a `DayOfWeek` with `Sunday = 0`, so the solution changes 0 to 7. `ISOWeek.GetYear` and `ISOWeek.GetWeekOfYear` give the ISO week.
- **Go.** `time.Date(..., time.UTC).Weekday()` returns a `time.Weekday` with `Sunday = 0`. `t.ISOWeek()` returns the week-based year and the week.
- **Rust.** The standard library has no calendar date. The solution computes a day number with `days_from_civil` of Howard Hinnant. 1970-01-01, day 0, was a Thursday, so the weekday is `(days + 3).rem_euclid(7) + 1`.
- **C.** The same day arithmetic as Rust. The `%` operator of C keeps the sign of the left operand, so the solution adds 7 to a negative remainder. `mktime` also sets `tm_wday`, but it reads the fields as local time.
- **C++.** `std::chrono::weekday{sys_days{ymd}}` gives the weekday. `iso_encoding()` returns 1 to 7, and `c_encoding()` returns 0 to 6.
- **Bash.** GNU `date -u -d 2024-03-10 +%u` prints the ISO number. `+%G %V` prints the week-based year and the week.

The library functions give the same answer for each date:

```python run id=python-day-of-week
from datetime import date

for d in [date(2024, 3, 10), date(1970, 1, 1), date(2024, 12, 30), date(2027, 1, 1)]:
    iso = d.isocalendar()
    print(d, d.strftime("%A"), d.isoweekday(), f"{iso.year}-W{iso.week:02d}-{iso.weekday}")
```

```output for=python-day-of-week
2024-03-10 Sunday 7 2024-W10-7
1970-01-01 Thursday 4 1970-W01-4
2024-12-30 Monday 1 2025-W01-1
2027-01-01 Friday 5 2026-W53-5
```

Without a library, Zeller's congruence computes the weekday from the year, the month and the day. January and February count as the months 13 and 14 of the year before. The result `h` is 0 for Saturday, 1 for Sunday and so on to 6 for Friday. The usual formula has `- 2J`, which can make a negative number. The version below uses `+ 5J`, which gives the same remainder modulo 7:

```python run id=python-zeller
from datetime import date


def zeller(year, month, day):
    """Return 0 for Saturday, 1 for Sunday, ..., 6 for Friday."""
    if month < 3:
        month += 12
        year -= 1
    k, j = year % 100, year // 100
    return (day + 13 * (month + 1) // 5 + k + k // 4 + j // 4 + 5 * j) % 7


for d in [date(2024, 3, 10), date(1970, 1, 1), date(2000, 2, 29), date(1, 1, 1)]:
    h = zeller(d.year, d.month, d.day)
    iso = (h + 5) % 7 + 1
    print(d, h, iso, d.isoweekday())
```

```output for=python-zeller
2024-03-10 1 7 7
1970-01-01 5 4 4
2000-02-29 3 2 2
0001-01-01 2 1 1
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `a-sunday`, `a-monday` and `a-saturday`: Sunday is 7 and not 0.
- `the-epoch` and `before-the-epoch`: 1970-01-01 was a Thursday and 1969-12-31 a Wednesday. A day number before 1970 is negative.
- `leap-day`, `after-leap-day-2000`, `year-1900-is-not-leap` and `year-2100-is-not-leap`: a wrong leap year rule moves every later date by one day.
- `gregorian-reform` and `day-before-the-reform`: 1582-10-15 was a Friday. 1582-10-04 is a Monday in the proleptic Gregorian calendar. In the Julian calendar it was a Thursday, the day before 1582-10-15.
- `first-day` and `last-day`: 0001-01-01 is a Monday and 9999-12-31 a Friday.
- `year-99`: 0099-12-31 was a Thursday. An API that reads the year 99 as 1999 gives a Friday.
- `iso-week-of-next-year` and `iso-week-of-last-year`: 2024-12-30 and 2027-01-01, where the week-based year is not the calendar year.
- `month-end`: 2024-04-30 was a Tuesday.

## Pitfalls

- **Sunday is 0 in many APIs.** JavaScript, C, C#, Go and `date +%w` number the days from 0 for Sunday. Python `weekday()` numbers them from 0 for Monday. A table of day names indexed with the wrong number shows the wrong day:

```python wrong id=python-weekday-from-zero
from datetime import date

sunday = date(2024, 3, 10)
print(sunday.weekday(), sunday.isoweekday())
print(sunday.strftime("%w %u %A"))
```

```output for=python-weekday-from-zero
6 7
0 7 Sunday
```

- **A date read in local time can be the day before.** `new Date("2024-03-10")` reads a date with no time as midnight UTC. `getDay()` returns the weekday in local time, and in New York midnight UTC is 19:00 of the day before:

```javascript wrong id=js-getday-local
process.env.TZ = "America/New_York";
const date = new Date("2024-03-10");
console.log(date.getDay());
console.log(date.getUTCDay());
console.log(date.toString());
```

```output for=js-getday-local
6
0
Sat Mar 09 2024 19:00:00 GMT-0500 (Eastern Standard Time)
```

- **The week-based year is not the calendar year.** In a Java pattern, `Y` is the week-based year and `y` is the year. The week rules of `Y` come from the locale. With `Locale.US`, a week starts on Sunday, and 2024-12-30 is in the first week of 2025:

```java wrong id=java-pattern-week-based-year
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.Locale;

public class Main {
    public static void main(String[] args) {
        LocalDate date = LocalDate.of(2024, 12, 30);
        System.out.println(date.format(DateTimeFormatter.ofPattern("YYYY-MM-dd", Locale.US)));
        System.out.println(date.format(DateTimeFormatter.ofPattern("yyyy-MM-dd", Locale.US)));
    }
}
```

```output for=java-pattern-week-based-year
2025-12-30
2024-12-30
```

In `strftime` and `date`, `%Y` is the calendar year and `%G` is the week-based year of `%V`. A week number with the wrong year is a week a year away:

```bash wrong id=bash-week-with-calendar-year
date -u -d 2024-12-30 '+%Y-W%V'
date -u -d 2024-12-30 '+%G-W%V'
```

```output for=bash-week-with-calendar-year
2024-W01
2025-W01
```

- **A week rule that is close to ISO 8601 is not ISO 8601.** The .NET documentation says that `Calendar.GetWeekOfYear` "does not map exactly to ISO 8601", even with `CalendarWeekRule.FirstFourDayWeek` and Monday. For 2007-12-31, a Monday in ISO week 1 of 2008, it returns 53:

```csharp wrong id=csharp-getweekofyear
using System.Globalization;

var date = new DateTime(2007, 12, 31);
var calendar = CultureInfo.InvariantCulture.Calendar;
Console.WriteLine(calendar.GetWeekOfYear(date, CalendarWeekRule.FirstFourDayWeek, DayOfWeek.Monday));
Console.WriteLine(ISOWeek.GetWeekOfYear(date));
Console.WriteLine(ISOWeek.GetYear(date));
```

```output for=csharp-getweekofyear
53
1
2008
```

- **The remainder of a negative number is negative in C.** A weekday from a day number before 1970 needs a remainder from 0 to 6. In C, Java, C#, Go, JavaScript and Rust `%` keeps the sign of the left operand. Python `%` and Rust `rem_euclid` do not:

```c wrong id=c-negative-remainder
#include <stdio.h>

int main(void) {
    long long days = -4;  // 1969-12-28, a Sunday
    printf("%lld\n", (days + 3) % 7 + 1);
    long long r = (days + 3) % 7;
    printf("%lld\n", (r < 0 ? r + 7 : r) + 1);
}
```

```output for=c-negative-remainder
0
7
```

## Related tasks

- [[compute-the-difference-of-two-dates]]: a day number, the base of the weekday arithmetic.
- [[add-days-to-a-date]]: the date of the next Monday or of a day a week later.
- [[check-for-a-leap-year]]: the rule that moves the weekdays of a year.
- [[parse-and-format-a-date]]: the patterns for day names and week numbers.
- [[convert-between-time-zones]]: the local date of an instant.

See [[calendar-arithmetic]] and [[iso-8601]] for the calendar and the week dates.

## References
