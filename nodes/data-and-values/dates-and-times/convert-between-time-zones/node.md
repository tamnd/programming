---
tag: NEW-convert-between-time-zones
kind: task
slug: convert-between-time-zones
name: Convert between time zones
aliases: [time zone conversion, convert a local time to another zone, convert to UTC, change the time zone, astimezone, withZoneSameInstant, TimeZoneInfo.ConvertTime, zoned_time, daylight saving time gap, ambiguous local time]
area: data-and-values/dates-and-times
level: core
summary: Convert a local date and time in one IANA time zone to the local time of the same instant in another zone. A local time in a gap or an overlap of daylight saving time needs a rule.
contract: contract.yaml
category: dates-and-times
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/zoneinfo.html
    title: The Python Standard Library, zoneinfo
    locator: Using ZoneInfo
  - url: https://peps.python.org/pep-0495/
    title: PEP 495, Local Time Disambiguation
    locator: Mind the Gap
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/time/ZonedDateTime.html
    title: Java SE 21 API, java.time.ZonedDateTime
    locator: the description of the class; withZoneSameInstant; withZoneSameLocal
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.timezoneinfo.converttime
    title: .NET API reference, TimeZoneInfo.ConvertTime
    locator: ConvertTime(DateTime, TimeZoneInfo, TimeZoneInfo), Exceptions and Remarks
  - url: https://pkg.go.dev/time
    title: Go standard library, time
    locator: func Date
  - url: https://en.cppreference.com/w/cpp/chrono/local_info
    title: cppreference.com, std::chrono::local_info
    locator: the description of the members
  - url: https://en.cppreference.com/w/cpp/chrono/choose
    title: cppreference.com, std::chrono::choose
    locator: the description of the enumeration
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/DateTimeFormat/DateTimeFormat
    title: MDN, Intl.DateTimeFormat() constructor
    locator: timeZone; Exceptions
  - url: https://www.rfc-editor.org/rfc/rfc9636.txt
    title: RFC 9636, The Time Zone Information Format (TZif)
    locator: Section 3.2, TZif Data Block; Section 3.3, TZif Footer
  - url: https://data.iana.org/time-zones/theory.html
    title: Theory and pragmatics of the tz code and data
    locator: Scope of the tz database
  - url: https://raw.githubusercontent.com/eggert/tz/main/etcetera
    title: The tz database, the source file etcetera
    locator: the comment before the Etc/GMT zones
  - url: https://man7.org/linux/man-pages/man3/tzset.3.html
    title: Linux manual page, tzset(3)
    locator: DESCRIPTION
  - url: https://man7.org/linux/man-pages/man1/date.1.html
    title: Linux manual page, date(1)
    locator: the format sequences %z and %:z
---

## Task

Convert a local date and time in one time zone to the local time of the same instant in another time zone. For example, 12:00 on 2024-07-01 in New York is 16:00 in UTC and 01:00 on the next day in Tokyo.

The contract `convert_time_zone(year, month, day, hour, minute, second, from_zone, to_zone)` takes the local time as six integers and the two zones as IANA names, such as `America/New_York`, `Asia/Kolkata` or `UTC`. It returns the local time in `to_zone` as text in the form `2024-07-02T01:00:00+09:00`. The offset at the end is the UTC offset of `to_zone` at that instant, and an offset of zero is `+00:00`.

The conversion has two steps:

1. Find the instant of the local time in `from_zone`. This step is hard, because the local clock jumps when daylight saving time starts or stops.
2. Find the offset of `to_zone` at that instant and add it. This step is easy, because each instant has exactly one offset in each zone.

A local time that a jump skips is in a **gap**: in New York, 02:30 on 2024-03-10 does not exist. A local time that occurs two times is in an **overlap**: 01:30 on 2024-11-03 occurs first with the offset -04:00 and then with -05:00. The contract uses this rule:

- A time in a gap moves forward by the length of the gap. 02:30 in the gap of New York is read with the offset before the gap, -05:00, so it is 07:30 UTC, which is 03:30 in New York.
- A time in an overlap uses the earlier of its two instants. 01:30 on 2024-11-03 in New York is 05:30 UTC.

Java uses this rule by default, and so does Python with `fold=0`. The years are 1970 to 2100. The tz database "organizes time zone and daylight saving time data by partitioning the world into timezones whose clocks all agree about timestamps that occur after the POSIX Epoch (1970-01-01 00:00:00 UTC)", so two names can give different results before 1970.

## Solutions

Most solutions find the instant with the same method. Read the local time as if it were UTC, and call that value `t`. Get the offsets of `from_zone` at `t - 1 day` and at `t + 1 day`. These are the two offsets that can apply to the local time. For each offset, the instant is `t - offset`. An instant is valid if the zone has that same offset at that instant. Take the earliest valid instant. If no instant is valid, the time is in a gap, and `t` minus the earlier offset moves it forward by the length of the gap. The method works when a zone changes its offset at most once in a day.

- **Python.** `datetime(..., tzinfo=ZoneInfo(from_zone)).astimezone(ZoneInfo(to_zone))`. The attribute `fold` is 0 by default, which gives the rule of the contract. `isoformat()` writes the offset as `+HH:MM`.
- **JavaScript.** A `Date` is an instant and has no zone. `Intl.DateTimeFormat` with the option `timeZone` gives the local date and time of an instant in a zone, and the difference to the UTC fields is the offset. The solution uses the method above to find the instant. The constructor throws a `RangeError` for an unknown name.
- **TypeScript.** The same code as JavaScript, with types.
- **Java.** `ZonedDateTime.of(local, ZoneId.of(from_zone)).withZoneSameInstant(ZoneId.of(to_zone))`. The pattern `uuuu-MM-dd'T'HH:mm:ssxxx` writes the offset, with `+00:00` for zero.
- **C#.** On Linux and macOS, `TimeZoneInfo.FindSystemTimeZoneById` takes IANA names. `ConvertTime` with a `DateTime` throws for a time in a gap, so the solution uses the method above with `GetUtcOffset`, and then converts a `DateTimeOffset`, which "cannot represent either an ambiguous time or an invalid time". The format `zzz` writes the offset.
- **Go.** `time.LoadLocation` reads the zone. `time.Date` does not promise a rule for a gap or an overlap, so the solution uses the method above. The layout `-07:00` writes `+00:00`, and the layout `Z07:00` writes `Z`.
- **Rust.** The standard library has no time zones, so the solution reads the TZif file of the zone from `/usr/share/zoneinfo`. A TZif file has a list of transitions and, after them, a footer with a POSIX TZ string such as `EST5EDT,M3.2.0,M11.1.0`. RFC 9636 says that "Local time for timestamps on or after the last transition is specified by the TZ string in the footer", so the solution uses the rule of that string for dates after the last transition in the file.
- **C.** The C library has only the zone of the process. The solution sets the variable `TZ`, calls `tzset` and `localtime_r`, and restores `TZ`. So the function is not safe to call from more than one thread.
- **C++.** `locate_zone(from_zone)->get_info(local)` returns a `local_info`. For a time in a gap, its member `first` is "the std::chrono::sys_info that ends just prior to the local_time", and for a time in an overlap it is the earlier one, so `local - first.offset` gives the rule of the contract. `std::format("{:%FT%T%Ez}", zoned_time{to_zone, instant})` writes the result.
- **Bash.** `TZ=zone date -d @seconds +%z` gives the offset of a zone at an instant, and the function uses the method above. The format sequence `%:z` of GNU date writes "+hh:mm numeric time zone".

With `fold=0`, Python moves a time in a gap forward and takes the earlier time in an overlap. PEP 495 says that in a gap, "the rules in effect before the transition should be used if fold=0". With `fold=1`, it uses the rules after the transition:

```python run id=python-fold
from datetime import datetime, timezone
from zoneinfo import ZoneInfo

ny = ZoneInfo("America/New_York")
for fold in (0, 1):
    gap = datetime(2024, 3, 10, 2, 30, fold=fold, tzinfo=ny)
    overlap = datetime(2024, 11, 3, 1, 30, fold=fold, tzinfo=ny)
    print(fold, gap.astimezone(timezone.utc).time(), overlap.astimezone(timezone.utc).time())
```

```output for=python-fold
0 07:30:00 05:30:00
1 06:30:00 06:30:00
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `nonexistent-time`: 02:30 on 2024-03-10 in New York is in the gap, so it gives 07:30 UTC.
- `ambiguous-time` and `ambiguous-time-london`: 01:30 on the day when daylight saving time stops, in New York and in London. Both give the earlier instant.
- `dst-dates-differ`: on 2024-03-20, New York is on daylight saving time and London is not yet, so the difference is 4 hours and not 5.
- `southern-hemisphere`: in January, Sydney is on daylight saving time with the offset +11:00.
- `half-hour-offset`, `45-minute-offset` and `half-hour-dst`: Kolkata has +05:30 and Kathmandu has +05:45. Lord Howe Island changes its offset by 30 minutes, from +10:30 to +11:00.
- `across-the-date-line`: Kiritimati has +14:00 and Pago Pago has -11:00, so noon on 2024-01-15 in Kiritimati is 11:00 on 2024-01-14 in Pago Pago.
- `before-skipped-day`, `skipped-day` and `after-skipped-day`: Samoa moved across the date line at the end of 2011, and 2011-12-30 does not exist in `Pacific/Apia`. A time on that day is in a gap of 24 hours, so it gives the same instant as the same time on 2011-12-31.
- `old-dst-rule`: in 2006, the United States started daylight saving time in April, so New York had -05:00 on 2006-03-15.
- `posix-sign`: `Etc/GMT+5` is 5 hours behind UTC. See the pitfalls.
- `result-before-epoch`: midnight UTC on 1970-01-01 is 19:00 on 1969-12-31 in New York.
- `far-future`: the TZif file of a zone can end at 2037, so a date in 2050 needs the rule in the footer of the file.
- `same-zone`: a conversion to the same zone keeps the local time.

## Pitfalls

- **A fixed offset in place of a zone.** An offset is right only for some dates. `-05:00` is the offset of New York in winter, but not in summer:

```python wrong id=python-fixed-offset
from datetime import datetime, timedelta, timezone
from zoneinfo import ZoneInfo

fixed = timezone(timedelta(hours=-5))
ny = ZoneInfo("America/New_York")
for month in (1, 7):
    a = datetime(2024, month, 15, 12, tzinfo=fixed).astimezone(timezone.utc)
    b = datetime(2024, month, 15, 12, tzinfo=ny).astimezone(timezone.utc)
    print(month, a.time(), b.time())
```

```output for=python-fixed-offset
1 17:00:00 17:00:00
7 17:00:00 16:00:00
```

- **The sign of `Etc/GMT+5`.** The names `Etc/GMT+N` use the sign of POSIX TZ strings, which is the opposite of an ISO 8601 offset. The tz source says that "TZ='Etc/GMT+4' uses the abbreviation "-04" and corresponds to 4 hours behind UT". In a POSIX TZ string, "The offset is positive if the local timezone is west of the Prime Meridian".

```python wrong id=python-etc-gmt-sign
from datetime import datetime, timezone
from zoneinfo import ZoneInfo

noon = datetime(2024, 1, 15, 12, tzinfo=timezone.utc)
print(noon.astimezone(ZoneInfo("Etc/GMT+5")).isoformat())
print(noon.astimezone(ZoneInfo("Etc/GMT-5")).isoformat())
```

```output for=python-etc-gmt-sign
2024-01-15T07:00:00-05:00
2024-01-15T17:00:00+05:00
```

- **Go `time.Date` in a gap or an overlap.** The Go documentation says: "In such cases, the choice of time zone, and therefore the time, is not well-defined. Date returns a time that is correct in one of the two zones involved in the transition, but it does not guarantee which." On this system, New York and London get different rules. In the gap, New York moves 02:30 back to 01:30, and London moves 01:30 forward to 02:30. In the overlap, New York takes the earlier instant and London the later one:

```go wrong id=go-date-gap-and-overlap
package main

import (
	"fmt"
	"time"
)

func main() {
	times := []struct {
		zone             string
		month, day, hour int
	}{
		{"America/New_York", 3, 10, 2},
		{"America/New_York", 11, 3, 1},
		{"Europe/London", 3, 31, 1},
		{"Europe/London", 10, 27, 1},
	}
	for _, c := range times {
		loc, err := time.LoadLocation(c.zone)
		if err != nil {
			panic(err)
		}
		t := time.Date(2024, time.Month(c.month), c.day, c.hour, 30, 0, 0, loc)
		fmt.Println(c.zone, t.Format("01-02 15:04 -07:00"), t.UTC().Format("15:04"))
	}
}
```

```output for=go-date-gap-and-overlap
America/New_York 03-10 01:30 -05:00 06:30
America/New_York 11-03 01:30 -04:00 05:30
Europe/London 03-31 02:30 +01:00 01:30
Europe/London 10-27 01:30 +00:00 01:30
```

Find the offsets yourself, as the solution does, when the rule matters.

- **C# `ConvertTime` with a `DateTime`.** The documentation says: "If the value of the dateTime parameter is an ambiguous local time, it is interpreted as a standard time. If the dateTime parameter is an invalid local time, this method throws an ArgumentException." So an overlap gives the later instant, and a gap gives an exception:

```csharp wrong id=csharp-converttime-gap
TimeZoneInfo ny = TimeZoneInfo.FindSystemTimeZoneById("America/New_York");
var overlap = new DateTime(2024, 11, 3, 1, 30, 0);
Console.WriteLine(TimeZoneInfo.ConvertTime(overlap, ny, TimeZoneInfo.Utc).ToString("HH:mm"));
var gap = new DateTime(2024, 3, 10, 2, 30, 0);
try
{
    TimeZoneInfo.ConvertTime(gap, ny, TimeZoneInfo.Utc);
}
catch (ArgumentException e)
{
    Console.WriteLine(e.GetType().Name);
}
```

```output for=csharp-converttime-gap
06:30
ArgumentException
```

- **C++ `choose::earliest` in a gap.** `choose` controls "how ambiguous or nonexistent local times should be resolved", but "For nonexistent local times, these two time points are identical." Both give the instant of the transition, 07:00 UTC, which is 03:00 in New York. The offset in `local_info::first` moves the time forward by the length of the gap:

```cpp wrong id=cpp-choose-in-a-gap
#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;
    const local_seconds gap = local_days{2024y / March / 10} + 2h + 30min;
    const zoned_time<seconds> earliest{"America/New_York", gap, choose::earliest};
    const zoned_time<seconds> latest{"America/New_York", gap, choose::latest};
    std::cout << std::format("{:%T}", earliest.get_sys_time()) << "\n";
    std::cout << std::format("{:%T}", latest.get_sys_time()) << "\n";
    const local_info info = locate_zone("America/New_York")->get_info(gap);
    std::cout << std::format("{:%T}", sys_seconds{gap.time_since_epoch() - info.first.offset}) << "\n";
}
```

```output for=cpp-choose-in-a-gap
07:00:00
07:00:00
07:30:00
```

- **The same local time in place of the same instant.** Java `withZoneSameLocal` keeps the clock reading and changes the instant. A conversion needs `withZoneSameInstant`:

```java wrong id=java-with-zone-same-local
import java.time.ZoneId;
import java.time.ZonedDateTime;

public class Main {
    public static void main(String[] args) {
        ZonedDateTime noon = ZonedDateTime.of(2024, 7, 1, 12, 0, 0, 0, ZoneId.of("America/New_York"));
        System.out.println(noon.withZoneSameLocal(ZoneId.of("Asia/Tokyo")));
        System.out.println(noon.withZoneSameInstant(ZoneId.of("Asia/Tokyo")));
    }
}
```

```output for=java-with-zone-same-local
2024-07-01T12:00+09:00[Asia/Tokyo]
2024-07-02T01:00+09:00[Asia/Tokyo]
```

- **Two libraries can read the same change in different ways.** Each library has its own code for the tz data, and a rare change can show a difference. On the test system, with the tz data 2026e, Python, Java, JavaScript and Go give -10:00 for `Pacific/Apia` at 22:00 UTC on 2011-12-29, and +14:00 after the change at 10:00 UTC on 2011-12-30. libstdc++ of GCC 13 and of GCC 15 gives +13:00 from 00:00 UTC on 2011-12-29. The C++ tests check the offset first and skip the cases for Apia when the library gives this result. Test the dates of rare changes in each library that you use.

- **An unknown zone name gives UTC with no error.** For the variable `TZ`, the C library says that if "its value cannot be interpreted using any of the formats specified below, then Coordinated Universal Time (UTC) is used." A misspelled name gives UTC, with part of the name as the abbreviation, and `date` exits with status 0:

```bash wrong id=bash-misspelled-zone
TZ=America/New_York date -d @1720000000 '+%H:%M %Z'
TZ=America/New_Yrok date -d @1720000000 '+%H:%M %Z'
echo "status $?"
```

```output for=bash-misspelled-zone
05:46 EDT
09:46 America
status 0
```

The C and Bash solutions check that the file of the zone exists. Python, Java, C#, Go and JavaScript throw or return an error for an unknown name.

## Related tasks

- [[get-the-current-date-and-time]]: the current instant in a zone.
- [[parse-and-format-a-date]]: text with an offset or a zone name.
- [[add-days-to-a-date]]: a day in a zone is not always 24 hours long.
- [[get-a-unix-timestamp]]: an instant as a number, with no zone.

See [[time-zone]] for offsets, gaps and overlaps, and [[tz-database]] for the source of the rules.

## References
