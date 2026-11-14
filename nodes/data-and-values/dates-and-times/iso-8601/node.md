---
tag: NEW-iso-8601
kind: standard
slug: iso-8601
name: ISO 8601
aliases: [ISO 8601 date format, ISO date, ISO timestamp, RFC 3339, RFC 9557, week date, ordinal date, ISO duration, ISO week]
area: data-and-values/dates-and-times
level: core
summary: >-
  ISO 8601 is the international standard for writing dates, times, durations and intervals as text, such as 2024-07-09T14:30:05+02:00. RFC 3339 is its profile for Internet timestamps.
see_also: [date-and-time-values, parse-and-format-a-date, time-zone, tz-database, unix-time]
sources:
- url: https://en.wikipedia.org/wiki/ISO_8601
  title: ISO 8601, Wikipedia
  locator: Introduction; History; General principles; Week dates; Durations; Time intervals
- url: https://www.rfc-editor.org/rfc/rfc3339.html
  title: "G. Klyne, C. Newman, RFC 3339: Date and Time on the Internet: Timestamps, July 2002"
  locator: Abstract; 4.3 Unknown Local Offset Convention; 5.6 Internet Date/Time Format
- url: https://www.rfc-editor.org/rfc/rfc9557.html
  title: "U. Sharma, C. Bormann, RFC 9557: Date and Time on the Internet: Timestamps with Additional Information, April 2024"
  locator: Abstract; 1 Introduction; 4 Syntax Extensions to RFC 3339
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date
  title: Date, MDN Web Docs
  locator: Date time string format
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/time/format/DateTimeFormatter.html
  title: Class DateTimeFormatter, Java SE 25 API
  locator: Predefined Formatters; Patterns for Formatting and Parsing
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/strftime.html
  title: strftime, The Open Group Base Specifications Issue 8 (POSIX.1-2024)
  locator: Description
- url: https://docs.python.org/3/library/datetime.html
  title: datetime, Basic date and time types, The Python Standard Library
  locator: date.fromisoformat; datetime.fromisoformat
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/toISOString
  title: Date.prototype.toISOString(), MDN Web Docs
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-date-and-time-format-strings
  title: Standard date and time format strings, .NET
  locator: The round-trip ("O", "o") format specifier
- url: https://pkg.go.dev/time
  title: Package time, Go standard library
  locator: Constants
---

## What it is

ISO 8601 is the international standard for dates and times in text. It "is an international standard covering the worldwide exchange and communication of date and time-related data."[^1] Its aim is to stop the confusion between local conventions, such as 07/09/2024, which is July 9 in the United States and 7 September in most of Europe. In ISO 8601 that date is `2024-07-09`.

The order is fixed: "the greatest temporal term (typically a year) is placed at the left and each successively lesser term is placed to the right of the previous term."[^1] So the text of two dates in the same form sorts in the order of time. The standard covers calendar dates, week dates, ordinal dates, times of day, offsets from UTC, durations, intervals and repeating intervals. Only numbers and a few letters and signs are allowed. Names such as `January` and `Thursday` "are not allowed in interchange representations within the standard."[^1]

Most programs use a smaller profile. RFC 3339 "defines a date and time format for use in Internet protocols that is a profile of the ISO 8601 standard for representation of dates and times using the Gregorian calendar."[^2] RFC 9557 extends that format "for representing additional information, including a time zone."[^3] JSON APIs, logs and databases use these profiles.

## Syntax or structure

**Dates.** One day has three forms:

| Form | Extended format | Basic format | Meaning |
|---|---|---|---|
| Calendar date | `2024-12-30` | `20241230` | year, month, day |
| Week date | `2025-W01-1` | `2025W011` | week-numbering year, week, day of the week (Monday is 1) |
| Ordinal date | `2024-365` | `2024365` | year, day of the year |

The extended format has separators, and the basic format has none. "The separator used between date values (year, month, week, and day) is the hyphen, while the colon is used as the separator between time values (hours, minutes, and seconds)."[^1] Reduced precision drops the parts on the right: `2024-07` is July 2024.

**Week dates.** Week 01 is "the week with the starting year's first Thursday in it (the formal ISO definition)"[^1]. Weeks start on Monday. Thus the week-numbering year can differ from the calendar year near 1 January. "If 1 January is on a Friday, Saturday or Sunday, it is in week 52 or 53 of the previous year (there is no week 00)."[^1] And 2024-12-30, a Monday, is in week 1 of 2025.

**Times and offsets.** A time is `hh:mm:ss`, with an optional decimal fraction, as in `14:30:05.12`. A date and a time join with `T`. A `Z` after the time means UTC: "Z is the zone designator for the zero UTC offset."[^1] Otherwise an offset such as `+02:00` or `-0400` follows. With no `Z` and no offset, the text is a local time with no known offset.

**Durations and intervals.** A duration has the form `PnYnMnDTnHnMnS` or `PnW`, so `P1Y2M10DT2H30M` is 1 year, 2 months, 10 days, 2 hours and 30 minutes. "P is the duration designator (for period) placed at the start of the duration representation."[^1] An interval joins two parts with `/`: a start and an end, a start and a duration, or a duration and an end. A repeating interval adds `R` and a count. The interval `P1Y2M10DT2H30M` repeated five times from `2008-03-01T13:00:00Z` is `R5/2008-03-01T13:00:00Z/P1Y2M10DT2H30M`[^1].

**RFC 3339.** Its grammar takes one form of ISO 8601: a full date, `T`, a full time with seconds and an offset or `Z`[^2]:

```text
full-date   = date-fullyear "-" date-month "-" date-mday
partial-time = time-hour ":" time-minute ":" time-second [time-secfrac]
time-offset = "Z" / time-numoffset
full-time   = partial-time time-offset
date-time   = full-date "T" full-time
```

It has no week dates, ordinal dates, basic format or reduced precision. It allows the second 60 for a leap second. It allows a space for `T`: "Applications using this syntax may choose, for the sake of readability, to specify a full-date and full-time separated by (say) a space character."[^2] It also gives a meaning to the offset `-00:00`, for a time whose UTC is known but whose offset to local time is not. In RFC 3339, `Z` and `+00:00` instead mean that UTC is the preferred reference point[^2].

**RFC 9557.** It adds suffixes in brackets after an RFC 3339 timestamp, such as a zone name: `2024-07-09T14:30:05+02:00[Europe/Paris]`. The RFC explains why the zone matters: "For example, 2020-01-01T00:00+01:00[Europe/Paris] will let programs add six months to the timestamp while adjusting for summer time (daylight saving time)."[^3] It also changes the meaning of `Z`, to align RFC 3339 "with the actual practice of interpreting the offset Z to mean the same as -00:00"[^3]. The reason is that ISO 8601:2000 and later "do not actually allow -00:00"[^3]. Now `Z` says that the local offset is unknown, and `+00:00` still says that UTC is the preferred reference point.

## Examples

The Python program below writes one day in the three date forms, in the extended and the basic format. It then reads five texts of one instant: with an offset, with `Z`, with another offset, in the basic format and as a week date.

```python run id=forms
from datetime import date, datetime, timedelta, timezone

d = date(2024, 12, 30)
year, week, weekday = d.isocalendar()
print(d.isoformat())                              # calendar date, extended format
print(d.strftime("%Y%m%d"))                       # calendar date, basic format
print(f"{year}-W{week:02d}-{weekday}")            # week date: this Monday is in week 1 of 2025
print(f"{d.year}-{d.timetuple().tm_yday:03d}")    # ordinal date: day 365 of a leap year

t = datetime(2024, 7, 9, 14, 30, 5, tzinfo=timezone(timedelta(hours=2)))
print(t.isoformat())
# Five texts for the same instant: offsets, Z, the basic format and a week date.
for s in ["2024-07-09T14:30:05+02:00", "2024-07-09T12:30:05Z", "2024-07-09T08:30:05-04:00",
          "20240709T123005Z", "2024-W28-2T12:30:05Z"]:
    print(s, datetime.fromisoformat(s) == t)
```

```output for=forms
2024-12-30
20241230
2025-W01-1
2024-365
2024-07-09T14:30:05+02:00
2024-07-09T14:30:05+02:00 True
2024-07-09T12:30:05Z True
2024-07-09T08:30:05-04:00 True
20240709T123005Z True
2024-W28-2T12:30:05Z True
```

The example files write and read ISO 8601 in each language. They show the week-numbering year of 2024-12-30, the forms that each parser rejects, and the durations where a standard library has them.

## Versions

The first edition, ISO 8601:1988, was published in June 1988[^1]. It replaced older ISO standards, such as ISO 2014 for all-numeric dates and ISO 2015 for the numbering of weeks. New editions followed in 2000 and 2004. In February 2019 the standard became two parts. ISO 8601-1:2019 has the basic rules. ISO 8601-2:2019 "is entirely new. It defines various extensions such as uncertainties or parts of the Extended Date/Time Format (EDTF)."[^1] An amendment to part 1 in 2022 brought back `24:00:00` for the end of a day[^1].

The text of the standard is sold by ISO, so most programmers read RFC 3339 and the documentation of their libraries instead. RFC 3339 was published in July 2002, and RFC 9557 updated it in April 2024[^3].

## Pitfalls

- **JavaScript reads a missing offset in two ways.** "When the time zone offset is absent, date-only forms are interpreted as a UTC time and date-time forms are interpreted as a local time."[^4] In New York, `new Date('2024-07-09')` is midnight UTC, but `new Date('2024-07-09T00:00')` is midnight local time, 4 hours later. MDN calls the first rule "a historical spec error that was not consistent with ISO 8601 but could not be changed due to web compatibility"[^4]. Always write the offset.
- **The week-numbering year in patterns.** In Java, the pattern letter `Y` is the "week-based-year"[^5], so the pattern `YYYY-MM-dd` writes 2024-12-30 as `2025-12-30`. Use `uuuu` or `yyyy`. In `strftime`, `%G` is the week-based year and `%Y` is the year[^6].
- **A local time is not an instant.** `2024-07-09T14:30:05` with no offset is valid ISO 8601, but it does not name one moment. RFC 3339 requires the offset. Store `Z` or an offset with each timestamp.
- **An offset is not a zone.** `+02:00` does not say that the time is in Paris. A program that adds six months to it gets the wrong local time in winter. Use the zone suffix of RFC 9557 or store the zone name. See [[time-zone]].
- **Parsers accept different subsets.** No common library accepts all of ISO 8601. Python's `date.fromisoformat` rejects reduced precision: "Reduced precision dates are not currently supported (YYYY-MM, YYYY)."[^7] Go's `time.RFC3339` layout needs the `T` and the offset. Agree on a profile, usually RFC 3339, and test the parser with each form you accept.
- **Fractions of seconds.** Libraries write different numbers of digits. `toISOString()` always writes milliseconds and is "always 24 or 27 characters long"[^8]. The .NET format `"o"` writes 7 digits, and Python writes 6 digits or none. Compare instants, not strings.
- **`Z` and `+00:00`.** Python writes `+00:00` for UTC, and JavaScript writes `Z`. Both are the same instant, but a check that compares text or expects one of them fails. Since RFC 9557, the two also differ in meaning: `Z` says that the local offset is unknown.
- **Basic offsets from `%z`.** `strftime` writes `+0530`, the basic format, even when the rest of the text uses the extended format. C++ has `%Ez` for `+05:30`.
- **Sorting text.** Text sorts in time order only when all values use the same form, the same offset and the same number of fraction digits. Convert to UTC and use one form before you sort strings.

## Tools and libraries

- Python: `datetime.isoformat()` and `fromisoformat()`. `fromisoformat()` reads most forms of ISO 8601; older versions read only the output of `isoformat()`[^7].
- JavaScript: `Date.prototype.toISOString()` and the format of `Date.parse`, "a simplified format based on ISO 8601"[^8]. Temporal reads and writes RFC 9557 and durations.
- Java: `DateTimeFormatter` has predefined formatters such as `ISO_LOCAL_DATE`, `ISO_OFFSET_DATE_TIME`, `ISO_WEEK_DATE`, `ISO_ORDINAL_DATE` and `BASIC_ISO_DATE`[^5]. `Duration` and `Period` read durations.
- .NET: the format specifier `"o"`, the "round-trip date/time pattern"[^9], `System.Globalization.ISOWeek`, and `XmlConvert` for durations.
- Go: the layouts `time.RFC3339` and `time.RFC3339Nano`, whose layout text is `2006-01-02T15:04:05Z07:00`[^10].
- C and C++: the `strftime` conversions `%F`, `%T`, `%G`, `%V`, `%u`, `%j` and `%z`, and in C++ `std::format` and `std::chrono::parse`.
- HTML: the `datetime` attribute of the `time` element holds a date, a time or a duration in a form close to ISO 8601.

## References
