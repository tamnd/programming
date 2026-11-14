---
tag: RPTZX9
kind: task
slug: compute-the-difference-of-two-dates
name: Compute the difference of two dates
aliases: [days between two dates, date difference, date diff, number of days between dates, subtract two dates, DAYS.between, DayNumber, timedelta days]
area: data-and-values/dates-and-times
level: intro
summary: Count the days from one calendar date to another. Subtract dates with no time zone or in UTC, because a local day can have 23 or 25 hours and a period in months is not a count of days.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/datetime.html
    title: The Python Standard Library, datetime
    locator: timedelta objects; Supported operations of date (timedelta = date1 - date2); Supported operations of datetime
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/UTC
    title: MDN Web Docs, Date.UTC()
    locator: Description
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/setUTCFullYear
    title: MDN Web Docs, Date.prototype.setUTCFullYear()
    locator: Description
  - url: https://tc39.es/proposal-temporal/docs/plaindate.html
    title: TC39 Temporal proposal, Temporal.PlainDate
    locator: date.until(other, options); date.since(other, options)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/temporal/ChronoUnit.html
    title: Java SE 25 API, java.time.temporal.ChronoUnit
    locator: DAYS
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/LocalDate.html
    title: Java SE 25 API, java.time.LocalDate
    locator: until(Temporal, TemporalUnit); until(ChronoLocalDate)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/Period.html
    title: Java SE 25 API, java.time.Period
    locator: between(LocalDate, LocalDate); getDays()
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.dateonly.daynumber?view=net-10.0
    title: .NET API reference, DateOnly.DayNumber
    locator: Definition
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.timespan.days?view=net-10.0
    title: .NET API reference, TimeSpan.Days
    locator: Remarks
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: func (Time) Sub; func (Time) Unix; type Duration
  - url: https://en.cppreference.com/w/cpp/chrono/system_clock.html
    title: cppreference.com, std::chrono::system_clock
    locator: sys_time, sys_days
  - url: https://howardhinnant.github.io/date_algorithms.html
    title: Howard Hinnant, chrono-Compatible Low-Level Date Algorithms
    locator: days_from_civil
  - url: https://man7.org/linux/man-pages/man1/date.1.html
    title: Linux manual page, date(1)
    locator: "-d, --date; -u, --utc"
---

## Task

Count the days from one calendar date to another. A date here has no time of day and no time zone, such as `2024-03-10`.

The contract `days_between(start_year, start_month, start_day, end_year, end_month, end_day)` returns the end date minus the start date as a number of days. The result is 0 for the same date, 1 for the next date, and negative when the end date is before the start date. The dates use the proleptic Gregorian calendar for the years 1 to 9999.

The usual method gives each date a day number and subtracts the two numbers. A day number counts the days from a fixed date, such as 1970-01-01 or 0001-01-01. The difference does not depend on the fixed date.

## Solutions

Each solution subtracts two dates with no time zone, or two midnights in UTC. In UTC every day has 86400 seconds, so a count of seconds divided by 86400 is exact. A count from two local times is not exact (see the pitfalls).

- **Python.** `date2 - date1` gives a `timedelta`, and its `days` attribute is the count. A `date` has no time and no time zone. `date.toordinal()` gives a day number with 0001-01-01 as day 1.
- **JavaScript.** `Date` has no type for a date alone, so the solution takes midnight UTC of each date, subtracts the two time values in milliseconds and divides by 86400000. It sets the year with `setUTCFullYear`, because `Date.UTC` reads the years 0 to 99 as 1900 to 1999. Where the Temporal API is available, `Temporal.PlainDate` has `until` and `since`.
- **TypeScript.** The same code as JavaScript, with the type `number`.
- **Java.** `ChronoUnit.DAYS.between(start, end)` returns a `long`. The documentation of `LocalDate.until` says that `start.until(end, DAYS)` is the same, and that "The calculation returns a whole number, representing the number of complete units between the two dates." `Period.between` is not a count of days (see the pitfalls).
- **C#.** `DateOnly.DayNumber` is the number of days since January 1, 0001. The solution returns `end.DayNumber - start.DayNumber`. `(end - start).Days` on two `DateTime` values also works when both are midnight of the same kind.
- **Go.** The solution makes both dates with `time.Date(..., time.UTC)` and divides the difference of `Unix()` by 86400. It does not use `end.Sub(start)`, because a `time.Duration` is an `int64` count of nanoseconds and holds about 292 years.
- **Rust.** The standard library has no calendar date. The solution computes a day number with the `days_from_civil` algorithm of Howard Hinnant and subtracts. A crate such as `chrono` or `jiff` has a date type with this subtraction.
- **C.** `mktime` reads the fields as local time. The solution uses the same `days_from_civil` arithmetic as Rust in `long long`.
- **C++.** `std::chrono::sys_days` is a `time_point` with a unit of one day. The solution converts each `year_month_day` to `sys_days`, and `(end - start).count()` is the number of days.
- **Bash.** GNU `date -u -d 2024-03-10 +%s` gives the seconds at midnight UTC. The solution subtracts the two values and divides by 86400. The `-u` option is necessary (see the pitfalls).

A date with no time zone gives the same count on each system:

```python run id=python-days-between
from datetime import date

print((date(2024, 3, 1) - date(2024, 1, 1)).days)
print((date(2023, 3, 1) - date(2024, 3, 1)).days)
print((date(9999, 12, 31) - date(1, 1, 1)).days)
```

```output for=python-days-between
60
-366
3652058
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `same-day`, `next-day` and `end-before-start`: the result is 0, 1 and -1.
- `common-year` and `leap-year`: 2023 has 365 days and 2024 has 366.
- `across-a-dst-change`: 2024-03-09 to 2024-03-11 is 2 days. In New York, 2024-03-10 had only 23 hours, but a date has no time zone.
- `across-leap-day` and `month-end-to-next-month`: 2024-01-31 to 2024-03-01 is 30 days, because February 2024 has 29 days.
- `year-1900-is-not-leap` and `year-2000-is-leap`: 1900 has 365 days and 2000 has 366.
- `400-years`: 400 Gregorian years always have 146097 days.
- `across-the-epoch`: 1969-12-31 to 1970-01-01 is 1 day. A day number before 1970 is negative, and the subtraction is the same.
- `gregorian-reform-gap`: 1582-10-04 to 1582-10-15 is 11 days in the proleptic Gregorian calendar. In the countries that changed calendar in October 1582, these two dates were consecutive days.
- `more-than-292-years`: 1700-01-01 to 2300-01-01 is 219145 days, more than a Go `time.Duration` holds.
- `whole-range` and `whole-range-backward`: 0001-01-01 to 9999-12-31 is 3652058 days, and the other direction is -3652058.
- `year-99`: 0099-12-31 to 0100-01-01 is 1 day. An API that reads the year 99 as 1999 gives a wrong count.

## Pitfalls

- **Local days are not 24 hours.** Two local midnights in a time zone with daylight saving time are not always a multiple of 86400 seconds apart. In New York, the clocks went from 02:00 to 03:00 on 2024-03-10:

```javascript wrong id=js-local-midnights
process.env.TZ = "America/New_York";
const start = new Date(2024, 2, 9);
const end = new Date(2024, 2, 11);
const days = (end - start) / (24 * 60 * 60 * 1000);
console.log(days);
console.log(Math.floor(days));
```

```output for=js-local-midnights
1.9583333333333333
1
```

`date -d` without `-u` reads the dates in the local time zone, and the shell division drops the 23 hours:

```bash wrong id=bash-local-midnights
export TZ=America/New_York
start=$(date -d 2024-03-10 +%s)
end=$(date -d 2024-03-11 +%s)
echo $(( end - start ))
echo $(( (end - start) / 86400 ))
```

```output for=bash-local-midnights
82800
0
```

- **The days field of a duration is not rounded to the nearest day.** Python keeps a `timedelta` as days, seconds and microseconds with `0 <= seconds < 3600*24`, so `days` goes toward the past for a negative difference. The difference of two date-times 12 hours apart has `days` 0 in one direction and -1 in the other:

```python wrong id=python-days-floor
from datetime import datetime

evening = datetime(2024, 3, 1, 18, 0)
morning = datetime(2024, 3, 2, 6, 0)
print((morning - evening).days)
print((evening - morning).days)
print(evening - morning)
```

```output for=python-days-floor
0
-1
-1 day, 12:00:00
```

The .NET documentation says: "The Days property represents whole days, whereas the TotalDays property represents whole and fractional days." For the same two times, `Days` is 0 in both directions:

```csharp wrong id=csharp-timespan-days
var evening = new DateTime(2024, 3, 1, 18, 0, 0);
var morning = new DateTime(2024, 3, 2, 6, 0, 0);
Console.WriteLine((morning - evening).Days);
Console.WriteLine((evening - morning).Days);
Console.WriteLine((evening - morning).Hours);
```

```output for=csharp-timespan-days
0
0
-12
```

Subtract dates, or midnights in UTC, and the count is exact.

- **A period is not a count of days.** Java `Period.between` gives years, months and days, and `getDays()` is only the days part. From 2024-01-01 to 2024-03-01 the period is two months and zero days:

```java wrong id=java-period-get-days
import java.time.LocalDate;
import java.time.Period;
import java.time.temporal.ChronoUnit;

public class Main {
    public static void main(String[] args) {
        LocalDate start = LocalDate.of(2024, 1, 1);
        LocalDate end = LocalDate.of(2024, 3, 1);
        Period period = Period.between(start, end);
        System.out.println(period);
        System.out.println(period.getDays());
        System.out.println(ChronoUnit.DAYS.between(start, end));
    }
}
```

```output for=java-period-get-days
P2M
0
60
```

- **A duration can overflow.** Go `Sub` returns the maximum `Duration` when the result does not fit, so the count of days is wrong with no error:

```go wrong id=go-sub-saturates
package main

import (
	"fmt"
	"time"
)

func main() {
	start := time.Date(1700, time.January, 1, 0, 0, 0, 0, time.UTC)
	end := time.Date(2300, time.January, 1, 0, 0, 0, 0, time.UTC)
	fmt.Println(end.Sub(start))
	fmt.Println(int64(end.Sub(start).Hours() / 24))
	fmt.Println((end.Unix() - start.Unix()) / 86400)
}
```

```output for=go-sub-saturates
2562047h47m16.854775807s
106751
219145
```

- **Two-digit years.** `Date.UTC` reads the years 0 to 99 as 1900 to 1999, so the year 99 becomes 1999:

```javascript wrong id=js-date-utc-two-digit-year
const msPerDay = 24 * 60 * 60 * 1000;
console.log(new Date(Date.UTC(99, 11, 31)).toISOString());
console.log((Date.UTC(100, 0, 1) - Date.UTC(99, 11, 31)) / msPerDay);
```

```output for=js-date-utc-two-digit-year
1999-12-31T00:00:00.000Z
-693959
```

## Related tasks

- [[add-days-to-a-date]]: the inverse operation, a date plus a number of days.
- [[find-the-day-of-the-week]]: a day number modulo 7 gives the weekday.
- [[check-for-a-leap-year]]: the rule that makes a year 365 or 366 days.
- [[measure-elapsed-time]]: the time between two instants with a monotonic clock.
- [[get-a-unix-timestamp]]: a date as seconds since 1970.
- [[convert-between-time-zones]]: the local date of an instant.

See [[calendar-arithmetic]] for the rules of the Gregorian calendar.

## References
