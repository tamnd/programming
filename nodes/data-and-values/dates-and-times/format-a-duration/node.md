---
tag: BJ4BQZ
kind: task
slug: format-a-duration
name: Format a duration
aliases: [format seconds as hours minutes and seconds, seconds to HH:MM:SS, format elapsed time, duration to string, format a time span, ISO 8601 duration, human-readable duration, Intl.DurationFormat, TimeSpan format, timedelta to string]
area: data-and-values/dates-and-times
level: intro
summary: Show a number of seconds as hours, minutes and seconds, such as 25:01:01. The hours do not wrap at 24, and a negative duration gets a minus sign. Also the ISO 8601 and human-readable forms.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/datetime.html
    title: The Python Standard Library, datetime
    locator: timedelta objects, Supported operations, str(t)
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/time/Duration.html
    title: Java SE 21 API, java.time.Duration
    locator: toString()
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/custom-timespan-format-strings
    title: .NET documentation, Custom TimeSpan format strings
    locator: the introduction, before the table of format specifiers
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-timespan-format-strings
    title: .NET documentation, Standard TimeSpan format strings
    locator: The constant ("c") format specifier
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: type Duration; func (Duration) String
  - url: https://en.cppreference.com/w/cpp/chrono/duration/formatter
    title: cppreference.com, std::formatter<std::chrono::duration>
    locator: Format specification, %H and %T
  - url: https://en.cppreference.com/w/cpp/chrono/hh_mm_ss
    title: cppreference.com, std::chrono::hh_mm_ss
    locator: the description of the class
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/DurationFormat
    title: MDN, Intl.DurationFormat
    locator: the description of the object
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/toISOString
    title: MDN, Date.prototype.toISOString()
    locator: Description
  - url: https://doc.rust-lang.org/std/time/struct.Duration.html
    title: The Rust Standard Library, std::time::Duration
    locator: checked_sub
  - url: https://en.wikipedia.org/wiki/ISO_8601
    title: Wikipedia, ISO 8601
    locator: Durations
  - url: https://man7.org/linux/man-pages/man1/date.1.html
    title: Linux manual page, date(1)
    locator: the format sequence %T
---

## Task

Show a duration, given as a whole number of seconds, as hours, minutes and seconds. For example, 3723 seconds is `01:02:03`, and 90061 seconds is `25:01:01`.

The contract `format_duration(seconds)` takes an integer from -10^15 to 10^15 and returns text in the form `HH:MM:SS`:

- `MM` and `SS` have two digits each, from `00` to `59`.
- `HH` is the whole number of hours, with at least two digits. The hours do not wrap at 24, and there is no field for days. So one day is `24:00:00` and one week is `168:00:00`.
- A negative duration starts with a minus sign, and the fields are those of the magnitude. So -61 seconds is `-00:01:01`.

A duration is an amount of time, such as the time that a job took. It is not a time of day, so a time-of-day formatter is the wrong tool. See the pitfalls.

The arithmetic is short. Take the magnitude `a` of the number. The hours are `a / 3600`, the minutes are `a % 3600 / 60`, and the seconds are `a % 60`, with integer division. Then put the sign in front.

There are two other common forms of a duration:

- **ISO 8601.** Durations "are represented by the format P[n]Y[n]M[n]DT[n]H[n]M[n]S or P[n]W". 90061 seconds is `PT25H1M1S`, or `P1DT1H1M1S` with a day field. A day in a calendar is not always 24 hours long, so the two forms are not always the same amount of time.
- **Human-readable text**, such as `25 hours, 1 minute, 1 second` or `25h1m1s`. The words and their order depend on the language.

## Solutions

- **Python.** `divmod(abs(seconds), 60)` and `divmod(minutes, 60)` give the fields, and the f-string format `02d` pads them. The test compares the result with `str(timedelta(seconds=n))` for durations that are shorter than a day.
- **JavaScript.** `Math.floor` divides, and `padStart(2, "0")` pads. A `number` holds every integer up to 2^53 exactly, so 10^15 is safe.
- **TypeScript.** The same code as JavaScript, with types.
- **Java.** `Duration.ofSeconds(seconds).abs()` gives the fields with `toHours()`, `toMinutesPart()` and `toSecondsPart()`. `toHours()` does not wrap at 24. `String.format` pads with `%02d`.
- **C#.** The function divides a `long` and pads with the format `00` and `CultureInfo.InvariantCulture`. The test shows that `TimeSpan.FromSeconds(1e15)` throws an `OverflowException`, so a `TimeSpan` cannot hold all of the range.
- **Go.** The function divides an `int64` and formats with `fmt.Sprintf("%s%02d:%02d:%02d", ...)`. It does not use `time.Duration`, which holds nanoseconds and overflows for large values. See the pitfalls.
- **Rust.** `i64::unsigned_abs` gives the magnitude as a `u64`, so the function is also correct for `i64::MIN`. The format `{:02}` of `format!` pads. `std::time::Duration` has no sign: `checked_sub` returns `None` "if the result would be negative".
- **C.** The caller gives a buffer of `FORMAT_DURATION_SIZE` bytes, and `snprintf` writes `%s%02llu:%02llu:%02llu`. The magnitude is an `unsigned long long`, so `LLONG_MIN` does not overflow.
- **C++.** `std::format("{:%T}", std::chrono::seconds{seconds})`. cppreference says that `%T` is "Equivalent to "%H:%M:%S"", and it describes `%H` as "the hour (24-hour clock)". For a duration, libstdc++ of GCC 15 does not wrap the hours, and it writes a minus sign for a negative duration. The tests check `24:00:00`, `25:01:01` and the negative cases, so they find a library that wraps.
- **Bash.** Shell arithmetic with `/` and `%`, and `printf '%s%02d:%02d:%02d\n'`. The tests include 10^15 and -10^15.

The standard libraries also have other forms. A Go `time.Duration` writes itself "in the form "72h3m0.5s". Leading zero units are omitted.":

```go run id=go-duration-string
package main

import (
	"fmt"
	"time"
)

func main() {
	for _, s := range []int64{0, 60, 3723, 90061, -61} {
		fmt.Println(time.Duration(s) * time.Second)
	}
}
```

```output for=go-duration-string
0s
1m0s
1h2m3s
25h1m1s
-1m1s
```

Java `Duration.toString()` writes ISO 8601: "The format of the returned string will be PTnHnMnS, where n is the relevant hours, minutes or seconds part of the duration". For a negative duration, each part has a minus sign:

```java run id=java-duration-iso
import java.time.Duration;

public class Main {
    public static void main(String[] args) {
        for (long s : new long[] {3723, 90061, -61}) {
            Duration d = Duration.ofSeconds(s);
            Duration a = d.abs();
            String hms = String.format("%s%02d:%02d:%02d",
                    d.isNegative() ? "-" : "", a.toHours(), a.toMinutesPart(), a.toSecondsPart());
            System.out.println(d + " " + hms);
        }
    }
}
```

```output for=java-duration-iso
PT1H2M3S 01:02:03
PT25H1M1S 25:01:01
PT-1M-1S -00:01:01
```

In JavaScript, "The Intl.DurationFormat object enables language-sensitive duration formatting". It takes the fields, not a number of seconds, and it does not move seconds into minutes and hours. The style `digital` writes one digit for an hour less than 10:

```javascript run id=js-duration-format
const d = { hours: 25, minutes: 1, seconds: 1 };
for (const style of ["long", "short", "narrow", "digital"]) {
  console.log(new Intl.DurationFormat("en", { style }).format(d));
}
console.log(new Intl.DurationFormat("en", { style: "long" }).format({ seconds: 90061 }));
console.log(new Intl.DurationFormat("en", { style: "digital" }).format({ hours: 1, minutes: 1, seconds: 1 }));
```

```output for=js-duration-format
25 hours, 1 minute, 1 second
25 hr, 1 min, 1 sec
25h 1m 1s
25:01:01
90,061 seconds
1:01:01
```

In C++, a duration with no format writes its count and its unit. `hh_mm_ss` "splits a std::chrono::duration into a "broken down" time such as hours : minutes : seconds", and `%T` uses the same fields:

```cpp run id=cpp-format-t
#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;
    for (const seconds s : {3723s, 90061s, -61s}) {
        std::cout << std::format("{} {:%T}", s, s) << "\n";
    }
    const hh_mm_ss<seconds> parts{90061s};
    std::cout << parts.hours().count() << " " << parts.minutes().count() << " " << parts.seconds().count() << "\n";
}
```

```output for=cpp-format-t
3723s 01:02:03
90061s 25:01:01
-61s -00:01:01
25 1 1
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `just-under-an-hour` and `just-under-a-day`: 3599 and 86399 seconds give `00:59:59` and `23:59:59`. One more second carries into the next field.
- `one-day`, `more-than-a-day`, `hundred-hours`, `one-week` and `one-year`: the hours do not wrap at 24, and they can have more than two digits, such as `100:00:00` and `8760:00:00`.
- `minus-one-second`, `negative-minute`, `negative-hours` and `negative-day`: the sign comes first, and the fields are those of the magnitude, such as `-00:00:01`.
- `beyond-go-duration`: 10^10 seconds is about 317 years. A Go `time.Duration` cannot hold it, and the result is `2777777:46:40`.
- `largest` and `most-negative`: 10^15 and -10^15 seconds. A C# `TimeSpan` cannot hold them, and a JavaScript `number` holds them exactly.

## Pitfalls

- **Python `str(timedelta)`.** The form is "[D day[s], ][H]H:MM:SS[.UUUUUU] , where D is negative for negative t". So a day is a separate field, the hours have one digit, and a negative duration has a negative number of days and positive hours. The documentation says: "Note that normalization of negative values may be surprising at first."

```python wrong id=python-timedelta-str
from datetime import timedelta

for s in (3723, 90061, -61):
    print(str(timedelta(seconds=s)))
```

```output for=python-timedelta-str
1:02:03
1 day, 1:01:01
-1 day, 23:58:59
```

- **Division of a negative number.** In Python, `//`, `%` and `divmod` round down, so the remainder of a negative number is positive. In C, C++, Java, C#, Go, Rust and JavaScript, the remainder has the sign of the dividend. Both give wrong fields for a negative duration. Divide the magnitude and put the sign in front:

```python wrong id=python-divmod-negative
seconds = -61
print(divmod(seconds, 60))
minutes, s = divmod(seconds, 60)
hours, m = divmod(minutes, 60)
print(f"{hours:02d}:{m:02d}:{s:02d}")
```

```output for=python-divmod-negative
(-2, 59)
-1:58:59
```

- **A time-of-day formatter.** A `Date` is an instant, and `toISOString` writes its time of day. "The timezone is always UTC". Seconds from the epoch as a time of day wrap at 24 hours, and a negative duration gives a time on the day before:

```javascript wrong id=js-toisostring-wraps
for (const s of [3723, 90061, -61]) {
  console.log(new Date(s * 1000).toISOString().slice(11, 19));
}
```

```output for=js-toisostring-wraps
01:02:03
01:01:01
23:58:59
```

GNU `date` does the same with the format sequence `%T`, which is "time; same as %H:%M:%S":

```bash wrong id=bash-date-wraps
date -u -d @3723 +%T
date -u -d @90061 +%T
date -u -d @-61 +%T
```

```output for=bash-date-wraps
01:02:03
01:01:01
23:58:59
```

- **C# custom `TimeSpan` formats.** The format `hh` is "The number of whole hours in the time interval that aren't counted as part of days", so `hh\:mm\:ss` drops the days. The documentation says: "Custom TimeSpan format specifiers also don't include a sign symbol that enables you to differentiate between negative and positive time intervals." The standard format `c` keeps the sign, but it writes the days as a separate field:

```csharp wrong id=csharp-timespan-custom
foreach (int s in new[] { 3723, 90061, -61 })
{
    TimeSpan t = TimeSpan.FromSeconds(s);
    Console.WriteLine(t.ToString(@"hh\:mm\:ss") + " " + t.ToString("c"));
}
```

```output for=csharp-timespan-custom
01:02:03 01:02:03
01:01:01 1.01:01:01
00:01:01 -00:01:01
```

- **Go `time.Duration` overflows.** "A Duration represents the elapsed time between two instants as an int64 nanosecond count. The representation limits the largest representable duration to approximately 290 years." A multiplication with `time.Second` that overflows does not give an error. It gives a wrong duration:

```go wrong id=go-duration-overflow
package main

import (
	"fmt"
	"time"
)

func main() {
	var seconds int64 = 10_000_000_000
	fmt.Println(time.Duration(seconds) * time.Second)
	fmt.Println(seconds/3600, seconds%3600/60, seconds%60)
}
```

```output for=go-duration-overflow
-2346317h47m53.709551616s
2777777 46 40
```

With a constant in place of the variable, the Go compiler finds the overflow and stops.

## Related tasks

- [[measure-elapsed-time]]: get the duration of a job, which this task then shows.
- [[compute-the-difference-of-two-dates]]: a duration in days between two dates.
- [[parse-and-format-a-date]]: format an instant, which is a different thing from a duration.
- [[get-a-unix-timestamp]]: seconds since the epoch are an instant, not a duration.

See [[iso-8601]] for the duration form `PnYnMnDTnHnMnS`.

## References
