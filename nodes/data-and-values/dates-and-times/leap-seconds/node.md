---
kind: concept
slug: leap-seconds
name: Leap seconds
aliases: [leap second, intercalary second, 23:59:60, TAI, International Atomic Time, UT1, DUT1, leap smear, smeared time, UTC-SLS, continuous UTC, leap-seconds.list]
area: data-and-values/dates-and-times
level: advanced
summary: >-
  A leap second was an extra second, 23:59:60, that kept UTC near the rotation of the Earth. There were 27 from 1972 to 2016. Most software ignores them, and none are planned.
applies_to: core-languages
viz: optional
formal: none
see_also: [unix-time, date-and-time-values, time-zone, tz-database, iso-8601, measure-elapsed-time, parse-and-format-a-date]
sources:
- url: https://hpiers.obspm.fr/iers/bul/bulc/ntp/leap-seconds.list
  title: leap-seconds.list, IERS Earth Orientation Center, Paris Observatory
  locator: Header comments, ATOMIC TIME and LEAP SECOND; data lines
- url: https://data.iana.org/time-zones/tz-link.html
  title: Time zone and daylight saving time data, IANA
  locator: Leap seconds
- url: https://en.wikipedia.org/wiki/Leap_second
  title: Leap second, Wikipedia
  locator: Introduction; History; Implementation, Leap second smearing; Phase-out and future of leap seconds; Problems
- url: https://www.rfc-editor.org/rfc/rfc3339.html
  title: "G. Klyne, C. Newman, RFC 3339: Date and Time on the Internet: Timestamps, July 2002"
  locator: 5.6 Internet Date/Time Format; 5.7 Restrictions; Appendix D Leap Seconds
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap04.html
  title: POSIX.1-2024, Base Definitions, General Concepts
  locator: 4.19 Seconds Since the Epoch
- url: https://data.iana.org/time-zones/theory.html
  title: Theory and pragmatics of the tz code and data, IANA
  locator: Leap seconds; Accuracy of the tz database
- url: https://developers.google.com/time/smear
  title: Leap Smear, Public NTP, Google for Developers
  locator: Our proposed standard smear; Example of the standard smear; Other smears
- url: https://docs.python.org/3/library/time.html
  title: time, Time access and conversions, The Python Standard Library
  locator: Introduction; time.time; time.strftime
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date
  title: Date, MDN Web Docs
  locator: Static methods
- url: https://learn.microsoft.com/en-us/dotnet/api/system.datetimeoffset.tounixtimeseconds
  title: DateTimeOffset.ToUnixTimeSeconds Method, .NET API
  locator: Remarks
- url: https://doc.rust-lang.org/std/time/struct.SystemTime.html
  title: Struct SystemTime, The Rust Standard Library
  locator: Description
- url: https://pkg.go.dev/time
  title: Package time, Go standard library
  locator: Overview; Monotonic Clocks; func Parse
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/Instant.html
  title: Class Instant, Java SE 25 API
  locator: Time-scale
- url: https://en.cppreference.com/w/cpp/chrono/utc_clock
  title: std::chrono::utc_clock, cppreference.com
  locator: Description
- url: https://data.iana.org/time-zones/tzdb/NEWS
  title: News for the tz database, IANA
  locator: Release 2026f
---

## Idea

Civil time uses UTC. The seconds of UTC come from atomic clocks, but its days must follow the rotation of the Earth, and the rotation is not regular. "Atomic clocks are more stable than the rate of the earth's rotation"[^1]. So UTC sometimes added one second at the end of a day. The last minute of that day had 61 seconds, and a clock showed 23:59:60 before 00:00:00. "There were 27 leap seconds, ranging from 1972-06-30 23:59:60 through 2016-12-31 23:59:60 UTC."[^2]

Most software does not count leap seconds. Unix time and the time types of most languages give each day exactly 86400 seconds. Thus a difference of two timestamps across a leap second is one second short, and the second 23:59:60 has no value. A program that needs the true number of seconds uses a table of the leap seconds, as the run block does. It also shows that Python's `datetime` rejects the second 60.

```python run id=idea
import calendar
import time
from datetime import datetime

# The first days after the 27 leap seconds, from the IERS file leap-seconds.list.
STEPS = """1972-07 1973-01 1974-01 1975-01 1976-01 1977-01 1978-01 1979-01 1980-01
1981-07 1982-07 1983-07 1985-07 1988-01 1990-01 1991-01 1992-07 1993-07 1994-07
1996-01 1997-07 1999-01 2006-01 2009-01 2012-07 2015-07 2017-01""".split()
STARTS = [calendar.timegm((int(s[:4]), int(s[5:]), 1, 0, 0, 0)) for s in STEPS]

def tai_minus_utc(unix):
    # TAI - UTC was 10 s on 1972-01-01 and grew by 1 s at each leap second.
    return 10 + sum(1 for s in STARTS if unix >= s)

a = calendar.timegm((2016, 12, 31, 23, 59, 59))
b = calendar.timegm((2017, 1, 1, 0, 0, 0))
print(len(STEPS), tai_minus_utc(a), tai_minus_utc(b))
print("Unix difference:", b - a)
print("SI seconds:", (b + tai_minus_utc(b)) - (a + tai_minus_utc(a)))

# struct_time allows the second 60, but Unix time has no number for it.
leap = time.strptime("2016-12-31 23:59:60", "%Y-%m-%d %H:%M:%S")
print(leap.tm_sec, calendar.timegm(leap) == b)

try:
    datetime.fromisoformat("2016-12-31T23:59:60+00:00")
except ValueError as e:
    print("datetime:", type(e).__name__)
```

```output for=idea
27 36 37
Unix difference: 1
SI seconds: 2
60 True
datetime: ValueError
```

## How it works

**Three time scales.** International Atomic Time (TAI) counts SI seconds from atomic clocks and has no corrections. UT1 follows the rotation of the Earth. UTC ticks with TAI, but it stepped by whole seconds to stay near UT1. "Leap seconds are needed to keep the two time scales in agreement"[^1], which means a difference between UT1 and UTC of less than 0.9 seconds. In 1972 UTC was already 10 seconds behind TAI. After the 27 leap seconds, TAI is 37 seconds ahead of UTC[^3]. The data lines of the IERS file give this count for each date, from 10 on 1 January 1972 to 37 on 1 January 2017[^1].

**When they happened.** The Earth does not slow down at a regular rate, so nobody could compute the leap seconds far ahead. The IERS announced each one, usually about six months before[^3]. A leap second came at the end of June or December[^4]. RFC 3339 gives a rule for programs that write timestamps: "Applications should not generate timestamps involving inserted leap seconds until after the leap seconds are announced."[^4] "All were positive leap seconds, adding a second to a UTC day; while a negative leap second was theoretically possible, none ever occurred."[^3]

**In text.** A leap second is the second 60 of the last minute. The grammar of RFC 3339 allows it:

```text
time-second     = 2DIGIT  ; 00-58, 00-59, 00-60 based on leap second
                          ; rules
```

The value 58 is for a negative leap second. In a zone with an offset, the leap second has the local time of that offset, so `2016-12-31T23:59:60Z` is `2017-01-01T08:59:60+09:00` in Tokyo[^4].

**In counters.** POSIX defines Unix time with a fixed day: "As represented in seconds since the Epoch, each and every day shall be accounted for by exactly 86400 seconds."[^5] So a count of seconds has no number for 23:59:60. A system that steps its clock repeats a number: "two seconds in sequence will have the same counter value."[^3] The tz project describes the real behavior: "Although the POSIX clock officially stopped during an inserted leap second, at least one proposed standard had it jumping back a second instead; and in practice POSIX clocks more typically either progressed glacially during a leap second, or were slightly slowed while near a leap second."[^6] See [[unix-time]].

**Smearing.** A smear does not step the clock. It makes the clock run a little slow for some hours, so that the extra second is spread over many seconds. Google proposed one form for all: "We encourage anyone smearing leap seconds to use a 24-hour linear smear from noon to noon UTC"[^7]. In that smear, each second is 86401/86400 of an SI second. "The change for the smear is about 11.6 ppm."[^7] At the start of the leap second, the smeared clock is almost 0.5 seconds behind UTC. When the leap second ends, it is almost 0.5 seconds ahead, and at noon the two agree again[^7]. The Python example computes these values.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `datetime` has no leap seconds; `time.struct_time` and `strptime` allow the second 60 |
| JavaScript | native | `Date` counts milliseconds with leap seconds ignored |
| TypeScript | native | the JavaScript `Date` and `Temporal`, with no leap seconds |
| Java | stdlib | the Java Time-Scale with 86400 seconds in each day; `Instant.parse` and `parsedLeapSecond()` |
| Kotlin | stdlib | `java.time` on the JVM; kotlinx-datetime also ignores them |
| C# | stdlib | `DateTime` and `DateTimeOffset` with 86400 seconds in each day |
| Go | stdlib | `time.Time` with no leap seconds; a monotonic reading for durations |
| Rust | library | `SystemTime` does not count them; the crates chrono and hifitime |
| C | stdlib | `tm_sec` up to 60; `time_t` with 86400 seconds in each day |
| C++ | stdlib | `utc_clock`, `tai_clock`, `gps_clock`, `clock_cast` and `get_leap_second_info` |
| Bash | idiom | GNU `date` rejects the second 60; the `right/` zone files |
| PowerShell | stdlib | the .NET `DateTime`, with 86400 seconds in each day |
| SQL | idiom | no leap seconds in timestamps; a table of leap seconds for exact intervals |
| Regex | n/a | no time values; a pattern can allow the second 60 |
| HTML | n/a | the valid time string has the seconds 00 to 59 only |
| CSS | n/a | no time scales |

Almost all languages follow POSIX and ignore leap seconds. In Python, "On Windows and most Unix systems, the leap seconds are not counted towards the time in seconds since the epoch"[^8]. In JavaScript, `Date.now()` returns milliseconds since 1970 "with leap seconds ignored"[^9]. In .NET, `ToUnixTimeSeconds` has the note "It does not take leap seconds into account."[^10] In Rust, "A SystemTime does not count leap seconds."[^11] Go states it for the whole package: "The calendrical calculations always assume a Gregorian calendar, with no leap seconds."[^12]

The parsers differ for the second 60. In Go, "Timestamps representing leap seconds (second 60) cannot be parsed."[^12] Python's `datetime`, JavaScript, C# and GNU `date` also reject it. The C `struct tm` allows it, and Python's `time.strftime` documents that "value 60 is valid in timestamps representing leap seconds and value 61 is supported for historical reasons."[^8]

Java defines its own scale: "The Java Time-Scale divides each calendar day into exactly 86400 subdivisions, known as seconds."[^13] Near a leap second it follows UTC-SLS: "On days that do have a leap second, the leap second is spread equally over the last 1000 seconds of the day, maintaining the appearance of exactly 86400 seconds per day."[^13] Its `Instant.parse` accepts `23:59:60` and returns `23:59:59`.

C++ is the only core language whose standard library counts leap seconds. Its `utc_clock` "measures time since 00:00:00 UTC, Thursday, 1 January 1970, including leap seconds."[^14] `clock_cast` converts between `system_clock`, `utc_clock`, `tai_clock` and `gps_clock` with the table of leap seconds in the tz database.

The example files show, in each language, how the second 60 parses and how long the interval from 23:59:59 to 00:00:00 is across the last leap second.

## Variations and related ideas

- **TAI and GPS time.** These scales have no leap seconds, so their seconds are continuous. "TAI is always exactly 19 seconds ahead of GPS time."[^3] A system that must not jump can keep TAI and convert to UTC only for display.
- **Other smears.** Smears differ in length and shape. Google's first smear in 2008 was a 20-hour cosine, and UTC-SLS uses the last 1000 seconds of the day[^7]. "This approach has led to differences between systems, as leap smear is not standardized and several different schemes were used in practice."[^3]
- **Zone files that count leap seconds.** The tz code can make `time_t` count leap seconds, with separate files, often in a directory `right/`. A process cannot choose for itself: "the leap-second choice is system-wide."[^6] A kernel that counts leap seconds needs the files that count them. See [[tz-database]].
- **Continuous UTC.** From 20 May 2027, UTC has no more leap seconds, and the difference between UT1 and UTC can grow to 3600 seconds[^3]. Unix time then matches UTC with no exceptions, but the table of the 27 old leap seconds stays necessary for timestamps from 1972 to 2016.
- **Elapsed time.** A monotonic clock measures durations with no steps. See [[measure-elapsed-time]].

## Pitfalls

- **Durations from the wall clock.** A step of the clock at a leap second can make the difference of two readings of the wall clock negative. At the leap second of 2016, the DNS resolver of Cloudflare got a negative number when it subtracted two values of Go's `time.Now()`. "This could have been avoided by using a monotonic clock source"[^3], and Go later added a monotonic reading to `time.Now()`. Measure durations with a monotonic clock.
- **Bugs that show only at a leap second.** A leap second came at most twice a year, so its code paths were not often tested. "A number of organizations reported problems caused by flawed software following the leap second that occurred on 30 June 2012."[^3] The same risk applies to any rare event in time code: test it with a fake clock.
- **The second 60 in input.** Data from GPS receivers, logs and other systems can contain `23:59:60`. Most parsers reject it, and Java changes it to `23:59:59`. Decide what the program does with it, and test the decision. See [[parse-and-format-a-date]].
- **Exact intervals from 1972 to 2016.** The difference of two Unix times is short by the number of leap seconds between them. From 1972-01-01 to 2017-01-01 the error is 27 seconds. Science, finance and navigation code that needs SI seconds must use TAI or a table of leap seconds.
- **Smeared and stepped clocks together.** During a smear, a smeared clock and a stepped clock disagree by up to about 0.5 seconds. Timestamps from two sources with different smears cannot be compared exactly in that window.
- **Tables that expire.** The IERS file has an expiry date, and older tools check it. Release 2026f of the tz database states: "No more leap seconds, and leap second tables no longer expire."[^15] A system with old software can still reject an old table, so update the software and the file together.

## Cost

Code that ignores leap seconds pays nothing: a day is 86400 seconds, and a conversion between a count and a date is arithmetic. Code that counts leap seconds needs a table. A conversion is a search in 27 sorted rows plus an addition, and the table is data that the system must install. The tz code needed space for future entries, but "localtime.c no longer supports 49 leap seconds, as 27 now suffice."[^15] A smear costs nothing in programs, but during the smear each second of the clock differs from an SI second by about 11.6 parts per million[^7].

## History

Before 1972, UTC kept close to UT by small changes in the rate of its seconds and by small steps[^3]. In 1972 the leap second replaced that method. The seconds of UTC became equal to SI seconds, and UTC was then 10 seconds behind TAI[^3]. The first leap second was on 30 June 1972[^2], and the IERS took over the schedule from the BIH in 1988[^3].

POSIX.1 defined seconds since the Epoch with 86400 seconds in each day, so most software never counted leap seconds[^5]. The tz code got support for them from Bradley White, but the tz project notes that it "was rarely used even before leap seconds were discontinued, because POSIX requires leap seconds to be excluded and many software packages would mishandle leap seconds if they were present."[^6] Google started to smear leap seconds in 2008[^7]. After the failures of 2012, 2015 and 2016, Go 1.9 added a monotonic clock reading to `time.Time`[^3], and C++20 added `utc_clock`[^14].

"Unfortunately they caused so many problems with civil timekeeping that the last leap second was in 2016."[^6] In November 2022, Resolution 4 of the 27th CGPM decided that the maximum difference between UT1 and UTC will be increased "in, or before, 2035."[^3] In October 2026, Resolution 5 of the 28th CGPM fixed the date: "Continuous UTC will become effective on 20 May 2027"[^3], with a limit of 3600 seconds. The tz link page states that "The leap second approach was discontinued in 2026 due to difficulties in implementing their use, and the current plan is to address the disagreement between UTC and UT1 in a different way, probably decades or centuries from now."[^2]

## References
