---
tag: 8AE4TK
kind: task
slug: add-days-to-a-date
name: Add days to a date
aliases: [date arithmetic, add months to a date, add years to a date, add a period, date plus days, subtract days from a date, next month, plusDays, AddDays, AddDate]
area: data-and-values/dates-and-times
level: intro
summary: Add days, months and years to a calendar date. Months and years need a rule for the end of the month, and a day in a time zone with daylight saving time is not always 24 hours.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/datetime.html
    title: The Python Standard Library, datetime
    locator: timedelta objects; Supported operations of datetime (datetime2 = datetime1 + timedelta)
  - url: https://docs.python.org/3/library/calendar.html
    title: The Python Standard Library, calendar
    locator: calendar.monthrange
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/setUTCMonth
    title: MDN Web Docs, Date.prototype.setUTCMonth()
    locator: Description
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/setUTCFullYear
    title: MDN Web Docs, Date.prototype.setUTCFullYear()
    locator: Description
  - url: https://tc39.es/proposal-temporal/docs/plaindate.html
    title: TC39 Temporal proposal, Temporal.PlainDate
    locator: date.add(duration, options)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/LocalDate.html
    title: Java SE 25 API, java.time.LocalDate
    locator: plusMonths(long); plusYears(long); plusDays(long)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/ZonedDateTime.html
    title: Java SE 25 API, java.time.ZonedDateTime
    locator: plusDays(long); plusHours(long)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.datetime.addmonths?view=net-10.0
    title: .NET API reference, DateTime.AddMonths
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.dateonly.addmonths?view=net-10.0
    title: .NET API reference, DateOnly.AddMonths
    locator: Returns
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: func (Time) AddDate; func Date
  - url: https://en.cppreference.com/w/cpp/chrono/year_month_day/operator_arith.html
    title: cppreference.com, std::chrono::year_month_day::operator+=, operator-=
    locator: Notes; Example
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/mktime.html
    title: The Open Group Base Specifications Issue 8, mktime
    locator: DESCRIPTION
  - url: https://www.gnu.org/software/coreutils/manual/html_node/Relative-items-in-date-strings.html
    title: GNU Coreutils manual, Relative items in date strings
    locator: the fuzz in units
---

## Task

Add a number of days, months and years to a calendar date, or subtract them. A date here has no time of day and no time zone, such as `2024-01-31`.

Days are a fixed unit of the calendar. Months and years are not: a month has 28 to 31 days, and a year has 365 or 366. Adding one month to January 31 gives February 31, which does not exist. Java, .NET and the Temporal API of JavaScript then use the last day of the month, February 29 in 2024. Go `AddDate`, the JavaScript `Date`, C `mktime` and GNU `date` move the extra days into the next month and give March 2.

The contract `add_to_date(year, month, day, years, months, days)` returns the new date as `YYYY-MM-DD` with this rule:

1. Add `years * 12 + months` months at once. Keep the day. If the day is after the last day of the new month, use the last day.
2. Then add the days.

The order is part of the rule. One year and then one month from 2024-02-29 gives 2025-02-28 and then 2025-03-28. Thirteen months at once gives 2025-03-29.

## Solutions

Each solution keeps the date in a type with no time zone, or in UTC. A date in local time can move by one hour on a day with a change of daylight saving time (see the pitfalls).

- **Python.** `date + timedelta(days=n)` adds days. The standard library has no month arithmetic, so the solution computes the new year and month from a count of months and uses `calendar.monthrange(year, month)[1]` to get the last day of the month.
- **JavaScript.** `Date` has no type for a date alone, so the solution uses the UTC methods. `setUTCFullYear(year, monthIndex, day)` accepts a day or a month out of range and moves into the next months. Day 0 of a month is the last day of the month before. The solution computes the last day this way and limits the day to it. Where the Temporal API is available, `Temporal.PlainDate` has `add({ months: 1 })`, and its default `overflow: 'constrain'` uses the last day of the month.
- **TypeScript.** The same code as JavaScript, with the type `number`.
- **Java.** `LocalDate.plusMonths(n)` uses the last valid day of the month when the day is not valid, and `plusDays(n)` adds days. The solution calls `plusMonths(years * 12L + months).plusDays(days)`. `LocalDate.plus(Period.of(years, months, days))` gives the same result.
- **C#.** `DateOnly.AddMonths(n)` and `DateTime.AddMonths(n)` use the last valid day of the resulting month. The documentation gives March 31 + 1 month = April 30. `AddDays(n)` adds days, and `AddYears(n)` adds years.
- **Go.** `t.AddDate(years, months, days)` adds all three at once and then normalizes the result, so one month after October 31 is December 1. The solution computes the new month itself, limits the day, and calls `AddDate(0, 0, days)` on a `time.Time` in UTC.
- **Rust.** The standard library has no calendar date. The solution converts the date to a count of days and back with the `days_from_civil` and `civil_from_days` algorithms of Howard Hinnant. A crate such as `chrono` or `jiff` has a date type with month arithmetic.
- **C.** `mktime` accepts fields out of range and normalizes them, so it moves January 31 + 1 month to March. It also reads the fields as local time. The solution uses the same day arithmetic as Rust, with a division that goes toward the past.
- **C++.** `year_month_day + months{n}` keeps the day, and the result is not `ok()` for a day such as February 30. The solution then uses `year / month / last`. `sys_days{ymd} + days{n}` adds days, and `std::format("{:%F}", d)` writes the date.
- **Bash.** GNU `date -d '2024-01-31 +1 month'` moves the extra days into the next month. The GNU manual calls months and years "fuzzy units". The solution does the month arithmetic in the shell and adds the days to UTC seconds with `date -u -d @SECONDS +%F`.

A date with no time zone gives the same result on each system:

```python run id=python-add-to-date
import calendar
from datetime import date, timedelta


def add_months(start, months):
    year, month0 = divmod(start.year * 12 + start.month - 1 + months, 12)
    last = calendar.monthrange(year, month0 + 1)[1]
    return start.replace(year=year, month=month0 + 1, day=min(start.day, last))


print(date(2024, 3, 10) + timedelta(days=30))
print(add_months(date(2024, 1, 31), 1))
print(add_months(date(2024, 2, 29), 12))
print(add_months(date(2024, 2, 29), 13))
```

```output for=python-add-to-date
2024-04-09
2024-02-29
2025-02-28
2025-03-29
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `nothing-to-add`: 2024-02-29 with nothing added stays 2024-02-29.
- `month-end-to-leap-february`, `month-end-to-short-february` and `month-end-to-30-day-month`: January 31 + 1 month is February 29 in 2024 and February 28 in 2023, and March 31 + 1 month is April 30.
- `back-from-month-end` and `back-to-leap-february`: May 31 - 1 month is April 30, and 2024-03-31 - 1 month is 2024-02-29.
- `leap-day-plus-one-year` and `leap-day-plus-four-years`: 2024-02-29 + 1 year is 2025-02-28, and + 4 years is 2028-02-29.
- `year-and-month-at-once` and `months-before-days`: 2024-02-29 + 1 year 1 month is 2025-03-29, and 2024-01-31 + 1 month 1 day is 2024-03-01.
- `month-into-next-year`, `thirteen-months-back` and `day-back-into-last-year`: the month and the year change together.
- `year-1900-is-not-leap` and `year-2100-is-not-leap`: 1900-02-28 + 1 day is 1900-03-01, and 2096-02-29 + 4 years is 2100-02-28.
- `many-days`, `many-days-back` and `many-months`: 1970-01-01 + 2932896 days is 9999-12-31, and the other two cases go from one end of the years 1 to 9999 to the other.

## Pitfalls

- **Normalization moves the day into the next month.** Go `AddDate`, `setUTCMonth` in JavaScript, C `mktime` and GNU `date` accept February 31 and make it March 2 or 3:

```go wrong id=go-adddate-overflow
package main

import (
	"fmt"
	"time"
)

func main() {
	jan31 := time.Date(2024, time.January, 31, 0, 0, 0, 0, time.UTC)
	oct31 := time.Date(2024, time.October, 31, 0, 0, 0, 0, time.UTC)
	fmt.Println(jan31.AddDate(0, 1, 0).Format(time.DateOnly))
	fmt.Println(oct31.AddDate(0, 1, 0).Format(time.DateOnly))
}
```

```output for=go-adddate-overflow
2024-03-02
2024-12-01
```

```bash wrong id=bash-date-month
date -u -d '2024-01-31 +1 month' +%F
date -u -d '2022-12-31 -1 month' +%F
```

```output for=bash-date-month
2024-03-02
2022-12-01
```

```c wrong id=c-mktime-normalize
#include <stdio.h>
#include <time.h>

int main(void) {
    struct tm tm = {0};
    tm.tm_year = 2024 - 1900;
    tm.tm_mon = 0;  // January
    tm.tm_mday = 31;
    tm.tm_hour = 12;
    tm.tm_isdst = -1;
    tm.tm_mon += 1;  // February 31
    mktime(&tm);
    printf("%04d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}
```

```output for=c-mktime-normalize
2024-03-02
```

The libraries that limit the day give the last day of the month:

```csharp run id=csharp-addmonths
var march31 = new DateOnly(2024, 3, 31);
Console.WriteLine(march31.AddMonths(1).ToString("yyyy'-'MM'-'dd"));
Console.WriteLine(march31.AddMonths(-1).ToString("yyyy'-'MM'-'dd"));
Console.WriteLine(new DateOnly(2024, 1, 31).AddMonths(1).ToString("yyyy'-'MM'-'dd"));
```

```output for=csharp-addmonths
2024-04-30
2024-02-29
2024-02-29
```

- **Adding in steps gives a different date.** Each step limits the day again. A year and then a month from a leap day loses one day. A month and then a month back does not return to the start date:

```java wrong id=java-year-then-month
import java.time.LocalDate;

public class Main {
    public static void main(String[] args) {
        LocalDate leapDay = LocalDate.of(2024, 2, 29);
        System.out.println(leapDay.plusYears(1).plusMonths(1));
        System.out.println(leapDay.plusMonths(13));
        LocalDate jan31 = LocalDate.of(2024, 1, 31);
        System.out.println(jan31.plusMonths(1).minusMonths(1));
    }
}
```

```output for=java-year-then-month
2025-03-28
2025-03-29
2024-01-29
```

- **A day is not always 24 hours.** In a time zone with daylight saving time, the day of the change has 23 or 25 hours. In New York, the clocks went from 02:00 to 03:00 on 2024-03-10. Java `ZonedDateTime.plusDays` adds days to the local date and time, and `plusHours` adds a duration:

```java wrong id=java-day-is-not-24-hours
import java.time.ZoneId;
import java.time.ZonedDateTime;

public class Main {
    public static void main(String[] args) {
        ZonedDateTime noon = ZonedDateTime.of(2024, 3, 9, 12, 0, 0, 0, ZoneId.of("America/New_York"));
        System.out.println(noon.plusDays(1));
        System.out.println(noon.plusHours(24));
    }
}
```

```output for=java-day-is-not-24-hours
2024-03-10T12:00-04:00[America/New_York]
2024-03-10T13:00-04:00[America/New_York]
```

In JavaScript, a number of milliseconds added to a local `Date` is a duration, and `setDate` changes the local date:

```javascript wrong id=js-local-day
process.env.TZ = "America/New_York";
const midnight = new Date(2024, 2, 10);
const plusMillis = new Date(midnight.getTime() + 24 * 60 * 60 * 1000);
const plusDate = new Date(midnight);
plusDate.setDate(plusDate.getDate() + 1);
console.log(plusMillis.getDate(), plusMillis.getHours());
console.log(plusDate.getDate(), plusDate.getHours());
```

```output for=js-local-day
11 1
11 0
```

Python adds a `timedelta` to the date and time on the clock, with no time zone adjustments. The result is one day later on the clock but only 23 hours later:

```python run id=python-aware-plus-day
from datetime import datetime, timedelta, timezone
from zoneinfo import ZoneInfo

noon = datetime(2024, 3, 9, 12, 0, tzinfo=ZoneInfo("America/New_York"))
later = noon + timedelta(days=1)
print(later.isoformat())
print(later.astimezone(timezone.utc) - noon.astimezone(timezone.utc))
```

```output for=python-aware-plus-day
2024-03-10T12:00:00-04:00
23:00:00
```

To add days to a date, use a date type with no time zone, or do the arithmetic in UTC.

## Related tasks

- [[compute-the-difference-of-two-dates]]: the number of days or months between two dates.
- [[find-the-day-of-the-week]]: the weekday of the result.
- [[check-for-a-leap-year]]: the rule behind February 29.
- [[convert-between-time-zones]]: move an instant to another zone.
- [[get-a-unix-timestamp]]: a date as seconds since 1970.

See [[calendar-arithmetic]] for the rules of the Gregorian calendar.

## References
