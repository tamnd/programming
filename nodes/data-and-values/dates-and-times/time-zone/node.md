---
tag: 2M7FMR
kind: concept
slug: time-zone
name: Time zone
aliases: [time zone, timezone, zone, UTC offset, offset from UTC, daylight saving time, DST, summer time, zone ID, IANA zone name, local time]
area: data-and-values/dates-and-times
level: core
summary: >-
  A time zone is a region with rules that give the offset from UTC at each instant. An offset such as -05:00 is one value of those rules, so an offset is not a zone.
applies_to: core-languages
viz: optional
formal: none
see_also: [date-and-time-values, unix-time, parse-and-format-a-date, measure-elapsed-time]
sources:
- url: https://en.wikipedia.org/wiki/Time_zone
  title: Time zone, Wikipedia
  locator: Introduction; History, Railway time
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/ZoneId.html
  title: Class ZoneId, Java SE 25 API
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Temporal/ZonedDateTime
  title: Temporal.ZonedDateTime, MDN Web Docs
  locator: Description; Time zones and offsets; Ambiguity and gaps
- url: https://pkg.go.dev/time
  title: Package time, Go standard library
  locator: type Location; func LoadLocation
- url: https://data.iana.org/time-zones/theory.html
  title: Theory and pragmatics of the tz code and data, IANA
  locator: Scope of the tz database; Time zone abbreviations; Accuracy of the tz database
- url: https://en.wikipedia.org/wiki/Tz_database
  title: tz database, Wikipedia
  locator: Introduction; Example zone and rule lines
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/ZonedDateTime.html
  title: Class ZonedDateTime, Java SE 25 API
  locator: Description
- url: https://peps.python.org/pep-0495/
  title: PEP 495, Local Time Disambiguation
  locator: Abstract
- url: https://docs.python.org/3/library/zoneinfo.html
  title: zoneinfo, IANA time zone support, The Python Standard Library
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/api/system.timezoneinfo
  title: TimeZoneInfo Class, .NET API
  locator: Definition; Methods
- url: https://en.cppreference.com/w/cpp/chrono/zoned_time
  title: std::chrono::zoned_time, cppreference.com
  locator: Description
- url: https://www.postgresql.org/docs/current/datatype-datetime.html
  title: Date/Time Types, PostgreSQL Documentation
  locator: 8.5.1.3 Time Stamps
---

## Idea

A clock on a wall shows local time, and local time differs from place to place. "A time zone is an area which observes a uniform standard time for legal, commercial, and social purposes."[^1] The difference between local time and UTC is the offset. New York is 5 hours behind UTC in winter, so its offset is -05:00. But in summer New York uses daylight saving time, and its offset is -04:00.

Thus a zone is not one offset. A zone is a set of rules that gives the offset for each instant. Java states the two kinds of value: "There are two distinct types of ID: Fixed offsets - a fully resolved offset from UTC/Greenwich, that uses the same offset for all local date-times Geographical regions - an area where a specific set of rules for finding the offset from UTC/Greenwich apply"[^2]. A program names a region with an identifier of the IANA time zone database, such as `America/New_York`.

```python run id=idea
from datetime import datetime, timedelta, timezone
from zoneinfo import ZoneInfo

ny = ZoneInfo("America/New_York")
# One zone, two offsets: the rules of New York give -05:00 in winter and -04:00 in summer.
for month in (1, 7):
    t = datetime(2024, month, 9, 12, tzinfo=timezone.utc).astimezone(ny)
    print(t.isoformat(), t.tzname())

# A fixed offset of -05:00 is right for New York in January and wrong in July.
fixed = timezone(timedelta(hours=-5))
print(datetime(2024, 7, 9, 12, tzinfo=timezone.utc).astimezone(fixed).isoformat())

# The same local time in two zones is two different instants.
paris = ZoneInfo("Europe/Paris")
print(datetime(2024, 7, 9, 9, 0, tzinfo=ny) - datetime(2024, 7, 9, 9, 0, tzinfo=paris))
```

```output for=idea
2024-01-09T07:00:00-05:00 EST
2024-07-09T08:00:00-04:00 EDT
2024-07-09T07:00:00-05:00
6:00:00
```

## How it works

**Offsets.** An offset is a fixed difference from UTC, written as `+09:00`, `-05:00` or `Z` for zero. Local time is UTC plus the offset. MDN shows the rule as `local time = UTC time + offset`, and its example is that UTC 1970-01-01T00:00:00 with the offset -05:00 is the local time 1969-12-31T19:00:00[^3]. "The offsets range from UTC−12:00 to UTC+14:00, and are usually a whole number of hours, but a few zones are offset by an additional 30 or 45 minutes, such as in India and Nepal."[^1] An instant with an offset is a complete value: it names one moment. See [[date-and-time-values]].

**Zones.** A zone maps each instant to an offset and an abbreviation. In Go, "A Location maps time instants to the zone in use at that time. Typically, the Location represents the collection of time offsets in use in a geographical area."[^4] The rules come from the tz database, which "attempts to record the history and predicted future of civil time scales."[^5] Its entries hold the rules over the years. For example, the United States has one rule for daylight saving time from 1987 to 2006 and another rule from 2007 on[^6]. So the offset of a zone depends on the date, and also on the version of the data. See [[tz-database]].

**From an instant to local time.** This direction is easy: find the rule for the instant, then add its offset. Each instant has exactly one local time in each zone. The Java documentation says: "Obtaining the offset for an instant is simple, as there is exactly one valid offset for each instant."[^7]

**From local time to an instant.** This direction has three cases, because the clocks jump twice a year. In Java's words: "Normal, with one valid offset."[^7] Then a gap: "This is when clocks jump forward typically due to the spring daylight savings change from "winter" to "summer". In a gap there are local date-time values with no valid offset."[^7] And an overlap: "In an overlap there are local date-time values with two valid offsets."[^7] In New York, 02:30 on 2024-03-10 does not exist, and 01:30 on 2024-11-03 happens two times. A library must pick a rule for each case.

**The choice in an overlap.** Python adds a flag to a local time. PEP 495 adds an attribute "that can be used to differentiate between two moments in time for which local times are the same."[^8] Its values are "0 and 1 with 0 corresponding to the earlier and 1 to the later of the two possible readings of an ambiguous local time."[^8] Java keeps the earlier offset by default and has `withLaterOffsetAtOverlap()`. C++ has `choose::earliest` and `choose::latest`, and Temporal has the option `disambiguation`.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `zoneinfo.ZoneInfo` for IANA zones and `datetime.timezone` for fixed offsets; the attribute `fold` |
| JavaScript | native | `Intl.DateTimeFormat` with the option `timeZone`; `Temporal.ZonedDateTime` |
| TypeScript | native | the JavaScript `Intl` and `Temporal` |
| Java | stdlib | `ZoneId`, `ZoneOffset`, `ZoneRules` and `ZonedDateTime` |
| Kotlin | stdlib | `java.time` on the JVM; `TimeZone` of kotlinx-datetime |
| C# | stdlib | `TimeZoneInfo` and `DateTimeOffset` |
| Go | stdlib | `time.LoadLocation`, `time.FixedZone` and the method `In` |
| Rust | library | no zones in the standard library; the crates chrono-tz and jiff |
| C | stdlib | the local zone of the process only, set by the variable `TZ`; `localtime()` and `mktime()` |
| C++ | stdlib | `std::chrono::time_zone`, `locate_zone()` and `zoned_time` |
| Bash | idiom | the variable `TZ` for one command or a subshell |
| PowerShell | stdlib | `Get-TimeZone` and the .NET `TimeZoneInfo` |
| SQL | native | `TIMESTAMP WITH TIME ZONE` and `AT TIME ZONE`, with different meanings in each system |
| Regex | n/a | no zones; a pattern can only match the text of an offset |
| HTML | n/a | the `datetime` attribute of `time` holds an offset in text, not a zone |
| CSS | n/a | no dates or zones |

Most standard libraries now read the IANA data. "The zoneinfo module provides a concrete time zone implementation to support the IANA time zone database as originally specified in PEP 615."[^9] In Go, `LoadLocation` takes a name such as `America/New_York`, which is "a location name corresponding to a file in the IANA Time Zone database"[^4]. In .NET, `TimeZoneInfo` "Represents any time zone in the world."[^10] In C++, "The class zoned_time represents a logical pairing of a time zone and a std::chrono::time_point whose resolution is Duration."[^11]

JavaScript has two levels. A `Date` is an instant, and only `Intl.DateTimeFormat` can show it in another zone. The newer `Temporal.ZonedDateTime` "is fundamentally represented as a combination of an instant, a time zone, and a calendar system."[^3] C has the weakest model: a process has one local zone, which the variable `TZ` sets, and no zone value exists. The Rust standard library has no zones at all.

SQL uses the name `TIMESTAMP WITH TIME ZONE`, but in PostgreSQL such a value does not keep a zone: "the value is stored internally as UTC, and the originally stated or assumed time zone is not retained."[^12]

The example files show New York in winter and in summer, and a gap or an overlap in each language that has a zone type.

## Variations and related ideas

- **Fixed offsets in data.** Formats such as RFC 3339 store an offset with each timestamp, such as `2024-07-09T08:00:00-04:00`. The offset shows the local time at that instant but not the rules of the zone. The format of RFC 9557 adds a zone name in brackets after the offset. See [[iso-8601]].
- **POSIX rule strings.** A string such as `EST5EDT,M3.2.0,M11.1.0` states one fixed rule for daylight saving time, with no history. The C example uses one.
- **The local zone of the system.** Each process has a default zone, from the variable `TZ` or the settings of the system. Functions such as `localtime` in C and `Date` getters in JavaScript use it.
- **Calendar arithmetic in a zone.** Adding 1 day to a zoned date and time adds a calendar day, which is 23 or 25 hours on the days when the clocks change. See [[calendar-arithmetic]].
- **Converting between zones.** The same instant shown in another zone. See [[convert-between-time-zones]].

## Pitfalls

- **An offset stored as a zone.** A meeting at 09:00 every Monday in New York has the offset -05:00 in winter and -04:00 in summer. A program that stores only `-05:00` moves the meeting by one hour in summer. Store the name `America/New_York`.
- **Abbreviations as identifiers.** The tz project warns that "these abbreviations are ambiguous in practice: e.g., CST means one thing in China and something else in North America, and IST can refer to time in India, Ireland or Israel."[^5] Parse and store zone names or numeric offsets, and use abbreviations only for display.
- **Future events stored in UTC.** The rules of a zone change by law. "For example, if today someone schedules a meeting for 13:00 next October 1, Dublin time, and tomorrow Ireland changes its daylight saving rules, software can mess up after the rule change if it blithely relies on conversions made before the change."[^5] For a future event in local time, store the local time and the zone name, and compute the instant when it is needed.
- **Days with 23 or 25 hours.** On the days when the clocks change, a local day is not 24 hours long. "In the worst case, an entire day may be missing from the local calendar."[^3] Do not add 86400 seconds to get the next day in a zone.
- **Local times in a gap or an overlap.** A user can enter a local time that does not exist or that exists two times. Each library has its own default, as the examples show. Choose the rule on purpose, and test the dates of the changes.
- **A zone column that drops the zone.** In PostgreSQL, `timestamp with time zone` stores an instant, not a zone. A program that needs the zone of the user must store the name in its own column.
- **The zone of the server.** Code that uses the local zone of the process gives different results on a laptop and on a server in UTC. Pass the zone explicitly.

## Cost

A conversion from an instant to local time is a search in the list of transitions of the zone, followed by an addition. The list is sorted by instant, so a binary search finds the rule. A conversion from local time to an instant also needs the gap and overlap checks. Most libraries cache the parsed rules, so the cost of a load of the zone data is paid once for each zone. The data of the whole database is a few hundred kilobytes in its compiled form, and some platforms do not ship it, so Python can use the package tzdata from PyPI instead: "By default, zoneinfo uses the system's time zone data if available; if no system time zone data is available, the library will fall back to using the first-party tzdata package available on PyPI."[^9]

## History

Before the railways, each town kept its own local mean time from the sun. In North America the railroads started standard time zones on Sunday, November 18, 1883[^1]. In 1884, Britain helped to gather international consent for global time, and countries moved to zones with offsets from Greenwich. In the United States, Congress made the zones law in the Standard Time Act of March 19, 1918[^1].

Daylight saving time adds a second offset in summer, and governments change its dates often. A POSIX `TZ` string holds one fixed rule for daylight saving time and no history. The tz database, started by Arthur David Olson, records the changes of each region since 1970[^6]. Its "uniform naming convention for entries in the database, such as America/New_York and Europe/Paris, was designed by Paul Eggert."[^6] Java 8 put ZoneId and ZoneRules into `java.time`, Python 3.9 added `zoneinfo`, and C++20 added `time_zone` and `zoned_time`[^11].

## References
