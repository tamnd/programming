---
tag: QSBRWK
kind: task
slug: check-for-a-leap-year
name: Check for a leap year
aliases: [leap year, is leap year, isleap, IsLeapYear, Year.isLeap, Gregorian leap year rule, February 29, the year 1900 problem]
area: data-and-values/dates-and-times
level: intro
summary: Check if a year is a leap year of the Gregorian calendar. A year divisible by 4 is a leap year, except a century year that is not divisible by 400, so 1900 was not a leap year and 2000 was.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://en.wikipedia.org/wiki/Gregorian_calendar
    title: Wikipedia, Gregorian calendar
    locator: the introduction
  - url: https://docs.python.org/3/library/calendar.html
    title: The Python Standard Library, calendar
    locator: calendar.isleap; calendar.leapdays
  - url: https://docs.python.org/3/library/datetime.html
    title: The Python Standard Library, datetime
    locator: datetime.MINYEAR; class date
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/Year.html
    title: Java SE 25 API, java.time.Year
    locator: the description of the class; isLeap(long)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.datetime.isleapyear?view=net-10.0
    title: .NET API reference, DateTime.IsLeapYear
    locator: Exceptions
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: func (Time) YearDay
  - url: https://en.cppreference.com/w/cpp/chrono/year/is_leap.html
    title: cppreference.com, std::chrono::year::is_leap
    locator: the description of the function
  - url: https://en.cppreference.com/w/cpp/chrono/year.html
    title: cppreference.com, std::chrono::year
    locator: the description of the class
  - url: https://learn.microsoft.com/en-us/troubleshoot/microsoft-365-apps/excel/wrongly-assumes-1900-is-leap-year
    title: Microsoft Learn, Excel incorrectly assumes that the year 1900 is a leap year
    locator: Summary; More information
  - url: https://en.wikipedia.org/wiki/Leap_year
    title: Wikipedia, Leap year
    locator: Gregorian calendar
---

## Task

Check if a year is a leap year of the Gregorian calendar. A leap year has 366 days, with February 29 as the extra day.

The rule has three parts:

1. A year divisible by 4 is a leap year,
2. except a year divisible by 100, which is not,
3. except a year divisible by 400, which is.

So 2024 and 2000 are leap years, and 2023, 1900 and 2100 are not. 400 Gregorian years have 97 leap years and 146097 days.

The contract `is_leap_year(year)` returns true or false. The years use the proleptic Gregorian calendar, in which the rule applies to all years, also the years before 1582. The numbering is astronomical, as in ISO 8601: year 0 is 1 BC, and year -1 is 2 BC. Year 0 is divisible by 400, so it is a leap year.

## Solutions

The rule fits in one expression: `year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)`. A test for a remainder of 0 is also right for negative years, because -4 % 4 is 0 in each language.

- **Python.** `calendar.isleap(year)` takes any integer. `calendar.leapdays(y1, y2)` counts the leap years from `y1` to `y2`, with `y2` not included.
- **JavaScript.** There is no function, so the solution writes the rule. The trick `new Date(year, 1, 29).getMonth() === 1` reads the years 0 to 99 as 1900 to 1999, so it gives false for the year 0.
- **TypeScript.** The same code as JavaScript, with the types `number` and `boolean`.
- **Java.** `Year.isLeap(long year)` is a static method. The documentation says that it checks "according to the ISO proleptic calendar system rules", and that the numbering has a year 0. `LocalDate.isLeapYear()` gives the same result for a date.
- **C#.** `DateTime.IsLeapYear(int year)` throws `ArgumentOutOfRangeException` when the year is less than 1 or greater than 9999. The solution writes the rule, and the tests compare it with `DateTime.IsLeapYear` for the years 1 to 9999.
- **Go.** The `time` package has no exported function for the rule, so the solution writes it. The tests check it against `YearDay()` of December 31, which is 366 in a leap year.
- **Rust.** The standard library has no calendar, so the solution writes the rule for `i64`.
- **C.** The C standard library has no function for the rule, so the solution writes it.
- **C++.** `std::chrono::year{y}.is_leap()` applies the rule. A `year` holds the years -32767 to 32767.
- **Bash.** The arithmetic command `(( ... ))` returns status 0 when the expression is not zero, so `is_leap_year` can be used in an `if` statement.

The library function of Python and the count of 400 years:

```python run id=python-isleap
import calendar

for year in [2024, 2023, 1900, 2000, 2100, 0, -4]:
    print(year, calendar.isleap(year))
print(calendar.leapdays(2000, 2400))
```

```output for=python-isleap
2024 True
2023 False
1900 False
2000 True
2100 False
0 True
-4 True
97
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `divisible-by-4` and `not-divisible-by-4`: 2024 is a leap year and 2023 is not.
- `century`, `next-century` and `year-1700`: 1900, 2100 and 1700 are divisible by 100 and not by 400.
- `divisible-by-400`, `year-1600` and `year-2400`: these century years are leap years.
- `year-1`, `year-4` and `year-100`: small years follow the same rule. An API that reads them as 1901, 1904 and 2000 can give the wrong answer for `year-100`.
- `year-1500-julian-leap` and `year-1582`: 1500 was a leap year in the Julian calendar, but not in the proleptic Gregorian calendar.
- `year-9999`: the last year of most date types.
- `year-zero`, `year-minus-1`, `year-minus-4`, `year-minus-100` and `year-minus-400`: year 0 and the negative years of astronomical numbering. Year 0 and -4 and -400 are leap years.

## Pitfalls

- **The rules in the wrong order.** A test for 4 that returns at once never reaches the tests for 100 and 400. The function below says that 1900 and 2100 are leap years:

```python wrong id=python-rules-in-wrong-order
def is_leap(year):
    if year % 4 == 0:
        return True
    if year % 100 == 0:
        return False
    return year % 400 == 0


print(is_leap(1900), is_leap(2100), is_leap(2000))
```

```output for=python-rules-in-wrong-order
True True True
```

Test the largest divisor first, or use the single expression of the rule.

- **The year 1900 in spreadsheets.** Microsoft says that "Microsoft Excel incorrectly assumes that the year 1900 is a leap year." Lotus 1-2-3 had this error first, and Excel kept it for compatibility. Excel has a February 29, 1900, and the document says that "The WEEKDAY function returns incorrect values for dates before March 1, 1900." The same document says that Excel "correctly handles all other leap years, including century years that are not leap years (for example, 2100)".

- **A date type with a limited range.** Python `date` and .NET `DateTime` hold only the years 1 to 9999. `calendar.isleap` works for year 0, but a date in year 0 raises an error:

```python wrong id=python-date-year-zero
import calendar
from datetime import date

print(calendar.isleap(0))
try:
    date(0, 2, 29)
except ValueError as e:
    print(type(e).__name__)
```

```output for=python-date-year-zero
True
ValueError
```

```csharp wrong id=csharp-isleapyear-range
try
{
    Console.WriteLine(DateTime.IsLeapYear(0));
}
catch (ArgumentOutOfRangeException e)
{
    Console.WriteLine(e.GetType().Name);
}
Console.WriteLine(DateTime.IsLeapYear(2000));
```

```output for=csharp-isleapyear-range
ArgumentOutOfRangeException
True
```

- **Historical dates used the Julian calendar.** The Julian calendar has a leap year every 4 years with no exception for century years. The countries of Europe changed to the Gregorian calendar between 1582 and 1923, so a historical date before the change in a country needs the Julian rule:

```python run id=python-julian-rule
import calendar


def is_julian_leap(year):
    return year % 4 == 0


for year in [1500, 1582, 1600, 1700]:
    print(year, is_julian_leap(year), calendar.isleap(year))
```

```output for=python-julian-rule
1500 True False
1582 False False
1600 True True
1700 True False
```

## Related tasks

- [[add-days-to-a-date]]: February 29 plus one year.
- [[compute-the-difference-of-two-dates]]: a year of 365 or 366 days.
- [[find-the-day-of-the-week]]: a leap day moves the weekdays of the rest of the year.
- [[parse-and-format-a-date]]: a parser that accepts or rejects February 29.

See [[calendar-arithmetic]] for the rules of the Gregorian calendar.

## References
