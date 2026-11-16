---
tag: T11RGW
kind: feature
owner: python
slug: dates-and-times
name: Dates and Times
aliases: [datetime, date, timedelta, timezone, zoneinfo, ZoneInfo, strftime, strptime, fromisoformat, aware and naive datetimes]
area: languages/python/io-and-os
level: intro
summary: >-
  The datetime module has date, time, datetime and timedelta. A datetime with a tzinfo is aware and names one instant, and one without is naive. zoneinfo supplies the IANA time zones.
grammar: "datetime.datetime(year, month, day, hour=0, minute=0, second=0, microsecond=0, tzinfo=None, *, fold=0)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/datetime.html", locator: "datetime module"}
see_also: [python/saving-structured-data-with-json, python/fancier-output-formatting, python/operating-system-interface]
sources:
  - url: https://docs.python.org/3.15/library/datetime.html
    title: datetime, Python 3.15 documentation
    locator: Aware and naive objects; Common properties; timedelta objects; date.weekday(); datetime objects; datetime.now(); datetime.utcnow(); fromisoformat(); strptime(); replace(); astimezone(); strftime() and strptime() behavior
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/stdlib.html#dates-and-times
    title: The Python Tutorial, 10. Brief tour of the standard library
    locator: 10.8. Dates and times
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/zoneinfo.html
    title: zoneinfo, Python 3.15 documentation
    locator: module introduction; Using ZoneInfo
    accessed: 2026-11-16
  - url: https://pkg.go.dev/time
    title: Package time, Go standard library
    locator: Overview; Constants (layouts); func LoadLocation; type Time
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import datetime as dt

birthday = dt.date(1964, 7, 31)
day = dt.date(2003, 12, 2)
print(day.strftime('%d %b %Y is a %A'))
print((day - birthday).days)

start = dt.datetime(2026, 3, 1, 9, 30)
print(start + dt.timedelta(days=2, hours=4))
print(start.isoformat())
```

```output for=synopsis
02 Dec 2003 is a Tuesday
14368
2026-03-03 13:30:00
2026-03-01T09:30:00
```

Dates subtract to a `timedelta`, a `datetime` plus a `timedelta` is a `datetime`, and `strftime()` and `isoformat()` turn them into text.

## Semantics

**r1.** "The datetime module supplies classes for manipulating dates and times."[^1] "While date and time arithmetic is supported, the focus of the implementation is on efficient member extraction for output formatting and manipulation. The module also supports objects that are timezone aware."[^2]

**r2.** "Date and time objects may be categorized as "aware" or "naive" depending on whether or not they include time zone information."[^1] "An aware object represents a specific moment in time that is not open to interpretation."[^1] "A naive object does not contain enough information to unambiguously locate itself relative to other date/time objects."[^1] "Objects of the date type are always naive."[^1]

**r3.** The date, datetime, time and timezone types share these features: "Objects of these types are immutable." and "Objects of these types are hashable, meaning that they can be used as dictionary keys."[^1] `replace()` will "Return a new datetime object with the same attributes, but with specified parameters updated."[^1]

**r4.** "A timedelta object represents a duration, the difference between two datetime or date instances."[^1] "Only days, seconds and microseconds are stored internally."[^1]

**r5.** "Subtraction of a datetime from a datetime is defined only if both operands are naive, or if both are aware. If one is aware and the other is naive, TypeError is raised."[^1] "Order comparison between naive and aware datetime objects raises TypeError."[^1] "Naive and aware datetime objects are never equal."[^1]

**r6.** `datetime.now(tz=None)` will "Return the current local date and time."[^1] "If tz is not None, it must be an instance of a tzinfo subclass, and the current date and time are converted to tz's time zone."[^1] "the recommended way to create an object representing the current time in UTC is by calling datetime.now(timezone.utc)."[^1]

**r7.** "Only one concrete tzinfo class, the timezone class, is supplied by the datetime module. The timezone class can represent simple time zones with fixed offsets from UTC".[^1] "The zoneinfo module provides a concrete time zone implementation to support the IANA time zone database".[^3] `ZoneInfo` objects "handle daylight saving time transitions with no further intervention".[^3] Example: [`zones`](#zones).

**r8.** `astimezone(tz)` will "Return a datetime object with new tzinfo attribute tz, adjusting the date and time data so the result is the same UTC time as self, but in tz's local time."[^1] "If self is naive, it is presumed to represent time in the system time zone."[^1]

**r9.** `strftime(format)` formats with codes such as `%Y`, "Year with century as a decimal number."[^1] `strptime(date_string, format)` will "Return a datetime corresponding to date_string, parsed according to format."[^1] `fromisoformat()` will "Return a datetime corresponding to a date_string in any valid ISO 8601 format".[^1] Example: [`parse`](#parse).

## Examples

```python run id=zones
import datetime as dt
from zoneinfo import ZoneInfo

la = ZoneInfo('America/Los_Angeles')
when = dt.datetime(2020, 10, 31, 12, tzinfo=la)
print(when, when.tzname())
later = when + dt.timedelta(days=1)
print(later, later.tzname())
print(later.astimezone(dt.timezone.utc))
print(later.astimezone(ZoneInfo('Asia/Tokyo')))
```

```output for=zones
2020-10-31 12:00:00-07:00 PDT
2020-11-01 12:00:00-08:00 PST
2020-11-01 20:00:00+00:00
2020-11-02 05:00:00+09:00
```

The day in between has the change from daylight saving time, so the offset moves from -07:00 to -08:00 (rule r7). `astimezone()` shows the same instant in other zones (rule r8).

```python run id=parse
import datetime as dt

a = dt.datetime.strptime('31/10/2026 18:05', '%d/%m/%Y %H:%M')
print(repr(a))
b = dt.datetime.fromisoformat('2026-10-31T18:05:00+02:00')
print(b.utcoffset(), b.astimezone(dt.timezone.utc).isoformat())
print(a.strftime('%Y-%m-%d %H:%M'), a.weekday())
```

```output for=parse
datetime.datetime(2026, 10, 31, 18, 5)
2:00:00 2026-10-31T16:05:00+00:00
2026-10-31 18:05 5
```

`strptime()` needs a format and gives a naive value. `fromisoformat()` keeps the offset in the text, so its result is aware (rules r2 and r9). `weekday()` returns "the day of the week as an integer, where Monday is 0 and Sunday is 6",[^1] so Saturday is 5.

```python run id=timedelta
import datetime as dt

d = dt.timedelta(hours=36, minutes=90)
print(d, d.days, d.seconds)
print(d.total_seconds() / 3600)
print(dt.timedelta(weeks=1) // dt.timedelta(days=1))
```

```output for=timedelta
1 day, 13:30:00 1 48600
37.5
7
```

The 37.5 hours become 1 day and 48600 seconds, because only days, seconds and microseconds are stored (rule r4).

## Pitfalls

A naive and an aware datetime do not mix:

```python fails id=mix expect=exception:TypeError
import datetime as dt

naive = dt.datetime(2026, 1, 1, 12)
aware = dt.datetime(2026, 1, 1, 12, tzinfo=dt.timezone.utc)
print(aware - naive)
```

Make both aware, for example with `replace(tzinfo=...)` or `astimezone()` (rule r5).

`utcnow()` returns a naive value. "Because naive datetime objects are treated by many datetime methods as local times, it is preferred to use aware datetimes to represent times in UTC."[^1] The docs mark it as deprecated.[^1] Use `datetime.now(timezone.utc)` (rule r6).

A fixed-offset `timezone` does not follow daylight saving time. `timezone(timedelta(hours=-8))` stays at -08:00 in summer. Use `ZoneInfo` for a named place (rule r7).

`zoneinfo` needs time zone data. It "uses the system's time zone data if available; if no system time zone data is available, the library will fall back to using the first-party tzdata package available on PyPI."[^3]

## Comparison

Go's "Package time provides functionality for measuring and displaying time."[^4] "A Time represents an instant in time with nanosecond precision."[^4] "Each time has an associated Location."[^4] So Go has no naive form. Go formats with a reference date, not `%` codes: "The reference time used in these layouts is the specific time stamp: 01/02 03:04:05PM '06 -0700".[^4] `time.LoadLocation` reads names "in the IANA Time Zone database, such as "America/New_York"", like `ZoneInfo` (rule r7).[^4]
