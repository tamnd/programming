---
tag: YTSBW9
kind: task
slug: parse-and-format-a-date
name: Parse and format a date
aliases: [parse an ISO 8601 timestamp, parse an RFC 3339 date-time, convert a timestamp to UTC, format a date as ISO 8601]
area: data-and-values/dates-and-times
level: core
summary: Parse an RFC 3339 date-time with a UTC offset, reject text that is not valid, and print the same instant in UTC with exactly three fraction digits.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://www.rfc-editor.org/rfc/rfc3339.html
    title: "RFC 3339, Date and Time on the Internet: Timestamps"
    locator: 4.2 Local Offsets, 4.3 Unknown Local Offset Convention, 5.6 Internet Date/Time Format, 5.7 Restrictions
  - url: https://docs.python.org/3/library/datetime.html
    title: The Python Standard Library, datetime, Basic date and time types
    locator: Aware and Naive Objects, datetime.fromisoformat, datetime.astimezone, datetime.isoformat
  - url: https://docs.python.org/3/library/re.html
    title: The Python Standard Library, re, Regular expression operations
    locator: \d, $, re.fullmatch
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date
    title: MDN Web Docs, Date
    locator: Date time string format, Date components and time zones
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/toISOString
    title: MDN Web Docs, Date.prototype.toISOString()
    locator: Return value, Exceptions
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/format/DateTimeFormatter.html
    title: Java SE 25 API, java.time.format.DateTimeFormatter
    locator: Patterns for Formatting and Parsing, ISO_OFFSET_DATE_TIME
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/format/ResolverStyle.html
    title: Java SE 25 API, java.time.format.ResolverStyle
    locator: STRICT, SMART, LENIENT
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.datetimeoffset.parseexact?view=net-10.0
    title: .NET API reference, DateTimeOffset.ParseExact
    locator: ParseExact(String, String[], IFormatProvider, DateTimeStyles), Remarks, Exceptions
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/custom-date-and-time-format-strings
    title: .NET, Custom date and time format strings
    locator: The "MM", "mm", "FFF", "fff", "zzz" and "yyyy" custom format specifiers
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/character-classes-in-regular-expressions
    title: .NET, Character classes in regular expressions
    locator: "Decimal digit character: \\d"
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/anchors-in-regular-expressions
    title: .NET, Anchors in regular expressions
    locator: "End of String or Line: $, End of String Only: \\z"
  - url: https://pkg.go.dev/time
    title: Go standard library, package time
    locator: Constants (RFC3339 and the layout rules), Parse
  - url: https://doc.rust-lang.org/std/time/index.html
    title: The Rust Standard Library, std::time
    locator: Structs
  - url: https://howardhinnant.github.io/date_algorithms.html
    title: Howard Hinnant, chrono-Compatible Low-Level Date Algorithms
    locator: days_from_civil, civil_from_days
  - url: https://en.cppreference.com/w/cpp/chrono/year_month_day/ok.html
    title: cppreference.com, std::chrono::year_month_day::ok
  - url: https://en.cppreference.com/w/cpp/chrono/system_clock/formatter.html
    title: cppreference.com, std::formatter<std::chrono::sys_time>
    locator: Format specification, %F, %T, %S
  - url: https://en.cppreference.com/w/c/language/operator_arithmetic.html
    title: cppreference.com, C arithmetic operators
    locator: Division
  - url: https://man7.org/linux/man-pages/man1/bash.1.html
    title: bash(1) manual page
    locator: Compound Commands ([[ expression ]], =~), ARITHMETIC EVALUATION
  - url: https://man7.org/linux/man-pages/man1/date.1.html
    title: date(1) manual page, GNU coreutils
    locator: -d, --date
  - url: https://man.freebsd.org/cgi/man.cgi?query=date&sektion=1
    title: FreeBSD manual page, date(1)
    locator: -j, -f
---

## Task

Read a timestamp in the date-time format of RFC 3339, for example `2026-11-10T19:30:00+07:00`. Print the same instant in UTC, as `2026-11-10T12:30:00.000Z`.

Each solution is a function `to_utc(timestamp)` that returns a string. The result has the form `YYYY-MM-DDTHH:MM:SS.sssZ`, with exactly three fraction digits. This is the form of JavaScript `toISOString`. When the input is not valid, the function reports an error and returns no result.

The input syntax is the `date-time` rule of RFC 3339 section 5.6. A date-time has a full date, the letter `T`, a time with seconds, an optional fraction, and an offset. The offset is `Z` for UTC, or a sign, two digits of hours, a colon and two digits of minutes. The hour is from 00 to 23. The day must exist in the month and the year, as the table of section 5.7 gives. Section 5.6 also lets `T` and `Z` be lower case, so `2026-11-10t12:00:00z` is valid.

The contract has three limits that RFC 3339 does not have. The fraction has 1 to 3 digits. The offset is from `-14:00` to `+14:00`, because .NET `DateTimeOffset` rejects an offset of more than 14 hours. The year of the input and the year of the result are from 0001 to 9999.

RFC 3339 section 4.2 calculates an offset as local time minus UTC. So the solution subtracts the offset: `19:30+07:00` is `12:30Z`. The time zone of the machine has no effect. Each test passes with any value of `TZ`.

## Solutions

Each solution checks the syntax first, and then it checks the calendar. A library parser alone is not sufficient, because each one accepts some text that RFC 3339 does not allow (see Pitfalls).

- **Python.** A pattern checks the syntax with `re.fullmatch`. The pattern uses `[0-9]`, because `\d` also matches digits of other scripts. `datetime.fromisoformat` then makes an aware `datetime`, and it raises `ValueError` for a day that is not in the calendar. `astimezone(timezone.utc)` changes the offset, and `isoformat(timespec="milliseconds")` gives three fraction digits.
- **JavaScript and TypeScript (Node.js).** A pattern checks the syntax, and a table of month lengths checks the day. `new Date(text)` then parses the instant, and `toISOString` prints it in UTC. The day check is necessary, because `Date` can move an invalid day into the next month. The TypeScript file adds types only.
- **Java.** A `Pattern` checks the syntax. `OffsetDateTime.parse` with `DateTimeFormatter.ISO_OFFSET_DATE_TIME` parses the text. That formatter uses `ResolverStyle.STRICT`, so it rejects a day that is not valid for the month. `withOffsetSameInstant(ZoneOffset.UTC)` changes the offset, and the pattern `uuuu-MM-dd'T'HH:mm:ss.SSS'Z'` prints the result.
- **C#.** A regular expression checks the syntax. It uses `[0-9]` and `\z`, because `\d` matches all Unicode decimal digits and `$` also matches before a final line end. `DateTimeOffset.ParseExact` with the formats `yyyy-MM-dd'T'HH:mm:ss.FFFzzz` and `yyyy-MM-dd'T'HH:mm:ss.FFF'Z'` parses the text. The second format has no offset specifier, so the call uses `DateTimeStyles.AssumeUniversal`, which gives the offset +00:00. With no style, the parser uses the offset of the local time zone.
- **Go.** A `regexp` checks the syntax, and `time.Parse(time.RFC3339, s)` parses the text. It rejects a day that is not in the month. The layout `2006-01-02T15:04:05.000Z07:00` prints the result. In a layout, a `Z` in place of the sign prints `Z` for UTC.
- **Rust.** `std::time` has `Duration`, `Instant` and `SystemTime`, and no calendar type. So the solution reads the digits by hand and counts days with the `days_from_civil` and `civil_from_days` algorithms of Howard Hinnant. These algorithms start the year on 1 March, so the leap day is the last day of a year. The function returns `Result<String, ParseError>`.
- **C.** The same hand parser and the same day algorithms. The function writes to a buffer of `TO_UTC_SIZE` bytes and returns 0, or it returns -1 for an error. A division in C truncates toward zero, so a `floor_div` function rounds the days down for an instant before 1970.
- **C++.** `std::regex` checks the syntax. `std::chrono::year_month_day::ok()` is false for a date that is not in the calendar. `sys_days` adds the time and subtracts the offset. `std::format("{:%FT%T}Z", t)` prints the result: `%F` is `%Y-%m-%d`, and `%T` prints the seconds with three digits for a `sys_time<milliseconds>`. The function returns `std::expected<std::string, std::string>`.
- **Bash.** `[[ $ts =~ $pattern ]]` matches a POSIX extended regular expression, and `BASH_REMATCH` holds the groups. The `10#` prefix reads `08` as eight, because a constant with a leading 0 is octal. Shell arithmetic then counts the days with the algorithms of Hinnant. The script does not use `date`, because its options to parse a date are different on each system: GNU `date` has `-d`, and BSD `date` has `-j` and `-f`. The function prints an error to standard error and returns status 1.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. The inputs that must give an error are in `cases/errors.jsonl`, and each test file holds them by name.

- `back-a-day`, `forward-a-day` and `year-end`: the offset moves the instant across midnight, and across the end of 2026.
- `leap-day`: 2024-02-29 is valid, and the offset moves it to 1 March.
- `half-hour-offset` and `quarter-hour-offset`: offsets such as `+05:30` and `+05:45` have minutes.
- `unknown-offset`: RFC 3339 section 4.3 uses `-00:00` when the time in UTC is known and the local offset is not. It is the same instant as `Z`.
- `fraction-one-digit` and `fraction-three-digits`: `.5` becomes `.500`.
- `before-epoch` and `before-epoch-fraction`: instants before 1970 are negative numbers. Each solution must round the day down, not toward zero.
- `largest-offset` and `smallest-offset`: `+14:00` and `-12:00`.
- `lower-case`: `t` and `z` are valid.
- `space-separator`: section 5.6 says that an application can choose a space in place of `T` for readability. This contract does not make that choice, so a space is an error.
- `year-one` and `year-9999`: the year has four digits, also when it is 0001.
- The errors: no offset, a date only, no seconds, `+0700`, `+07`, 2026-02-29, 2026-04-31, month 13, day 0, hour 24, minute 60, offset hour 24, offset minute 60, a comma before the fraction, a space at the end, and the empty string.
- `leap-second`: section 5.7 lets the second be 60 at the end of a month with a leap second. The library parsers reject it (see Pitfalls), so the contract makes it an error for each solution.

## Pitfalls

- **A timestamp with no offset.** The value is then not one instant. Python makes a naive `datetime`, and `astimezone` presumes that a naive value is in the time zone of the system. The same text gives two instants in two zones:

```python wrong id=python-naive
import os
import time
from datetime import datetime, timezone

for zone in ["Asia/Ho_Chi_Minh", "America/New_York"]:
    os.environ["TZ"] = zone
    time.tzset()
    value = datetime.fromisoformat("2026-11-10T12:00:00")
    print(zone, value.tzinfo, value.astimezone(timezone.utc).isoformat())
```

```output
Asia/Ho_Chi_Minh None 2026-11-10T05:00:00+00:00
America/New_York None 2026-11-10T17:00:00+00:00
```

JavaScript has a second rule. With no offset, a date-only form is UTC and a date-time form is local time:

```javascript wrong id=js-no-offset
process.env.TZ = "America/New_York";
console.log(new Date("2026-11-10").toISOString());
console.log(new Date("2026-11-10T00:00:00").toISOString());
```

```output
2026-11-10T00:00:00.000Z
2026-11-10T05:00:00.000Z
```

Require the offset, or write down which zone the text is in and give that zone to the parser.

- **A day that does not exist.** MDN says that implementations behave inconsistently for a date such as 30 February. Node.js moves it into March:

```javascript wrong id=js-rollover
console.log(new Date("2026-02-30T12:00:00Z").toISOString());
console.log(new Date("2026-04-31T12:00:00Z").toISOString());
```

```output
2026-03-02T12:00:00.000Z
2026-05-01T12:00:00.000Z
```

- **A parser that accepts too much.** The Go documentation says that `time.RFC3339` with `time.Parse` does not accept all the formats of the RFC, and that it accepts some formats that the RFC does not define:

```go wrong id=go-lenient
package main

import (
	"fmt"
	"time"
)

func main() {
	for _, s := range []string{
		"2026-11-10T12:00:00+24:00",
		"2026-11-10T12:00:00+07:60",
		"2026-11-10T12:00:00,5Z",
	} {
		t, err := time.Parse(time.RFC3339, s)
		fmt.Println(s, "->", t.UTC().Format(time.RFC3339Nano), err)
	}
}
```

```output
2026-11-10T12:00:00+24:00 -> 2026-11-09T12:00:00Z <nil>
2026-11-10T12:00:00+07:60 -> 2026-11-10T04:00:00Z <nil>
2026-11-10T12:00:00,5Z -> 2026-11-10T12:00:00.5Z <nil>
```

Python `fromisoformat` accepts any valid ISO 8601 form, with some exceptions, so it also accepts text that is not RFC 3339:

```python wrong id=python-lenient
from datetime import datetime

for text in ["2026-11-10T24:00:00+0700", "2026-11-10 12:00Z", "20261110T120000Z"]:
    print(text, "->", datetime.fromisoformat(text).isoformat())
```

```output
2026-11-10T24:00:00+0700 -> 2026-11-11T00:00:00+07:00
2026-11-10 12:00Z -> 2026-11-10T12:00:00+00:00
20261110T120000Z -> 2026-11-10T12:00:00+00:00
```

Check the syntax with a strict pattern before the call.

- **The wrong pattern letter.** The letters of a format are short, and a wrong letter gives a value that looks correct. In Java, `Y` is the week-based year, and `u` is the year:

```java wrong id=java-week-year
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.Locale;

public class Main {
  public static void main(String[] args) {
    LocalDate date = LocalDate.of(2026, 12, 28);
    System.out.println(date.format(DateTimeFormatter.ofPattern("YYYY-MM-dd", Locale.ROOT)));
    System.out.println(date.format(DateTimeFormatter.ofPattern("uuuu-MM-dd", Locale.ROOT)));
  }
}
```

```output
2027-12-28
2026-12-28
```

In .NET, `mm` is the minute, and `MM` is the month:

```csharp wrong id=csharp-minute
using System.Globalization;

var value = new DateTime(2026, 11, 10, 12, 34, 56, DateTimeKind.Utc);
Console.WriteLine(value.ToString("yyyy-mm-dd", CultureInfo.InvariantCulture));
Console.WriteLine(value.ToString("yyyy-MM-dd", CultureInfo.InvariantCulture));
```

```output
2026-34-10
2026-11-10
```

A Go layout is the reference time `Mon Jan 2 15:04:05 MST 2006`, written in the form to print. Text that is not part of the reference time is printed with no change:

```go wrong id=go-layout
package main

import (
	"fmt"
	"time"
)

func main() {
	t := time.Date(2026, 11, 10, 12, 0, 0, 0, time.UTC)
	fmt.Println(t.Format("YYYY-MM-DD"))
	fmt.Println(t.Format("2006-01-02"))
}
```

```output
YYYY-MM-DD
2026-11-10
```

- **The leap second.** RFC 3339 allows `23:59:60`, but the parsers of Python, JavaScript, Java, C# and Go reject it. Python gives this error:

```python run id=python-leap-second
from datetime import datetime

try:
    datetime.fromisoformat("2016-12-31T23:59:60+00:00")
except ValueError as error:
    print("ValueError:", error)
```

```output
ValueError: second must be in 0..59, not 60
```

## Related tasks

- [[sort-a-list-of-records]]: sort records by an instant. Text with different offsets does not sort in time order, so convert each value to UTC first.
- [[read-an-environment-variable]]: `TZ` is an environment variable, and the tests do not depend on it.
- [[split-a-string]]: split a line into fields before you parse the date in one of them.
- [[parse-command-line-arguments]]: read a timestamp from the command line.

## References
