---
tag: NEW-tz-database
kind: standard
slug: tz-database
name: IANA time zone database
aliases: [tz database, tzdb, tzdata, zoneinfo, Olson database, IANA Time Zone Database, TZif]
area: data-and-values/dates-and-times
level: core
summary: >-
  The IANA time zone database (tzdb) records the history of local time for regions of the world. Systems compile it into TZif files, and programs name its zones with IDs such as America/Chicago.
see_also: [time-zone, date-and-time-values, unix-time, parse-and-format-a-date]
sources:
- url: https://www.iana.org/time-zones
  title: Time Zone Database, IANA
  locator: Introduction; Latest version
- url: https://data.iana.org/time-zones/tz-link.html
  title: Time zone and daylight saving time data, IANA
  locator: Introduction; The tz database
- url: https://en.wikipedia.org/wiki/Tz_database
  title: tz database, Wikipedia
  locator: Introduction; Names of timezones; Unix-like systems; History
- url: https://data.iana.org/time-zones/theory.html
  title: Theory and pragmatics of the tz code and data, IANA
  locator: Scope of the tz database; Timezone identifiers
- url: https://data.iana.org/time-zones/tz-how-to.html
  title: Bill Seymour, How to Read the tz Database Source Files, IANA
  locator: Rule lines; Zone lines
- url: https://www.rfc-editor.org/rfc/rfc9636.html
  title: "A. Olson, P. Eggert, K. Murchison, RFC 9636: The Time Zone Information Format (TZif), October 2024"
  locator: 1 Introduction; 3 The Time Zone Information Format; 3.1 TZif Header
- url: https://www.rfc-editor.org/rfc/rfc6557.html
  title: "E. Lear, P. Eggert, RFC 6557: Procedures for Maintaining the Time Zone Database, February 2012"
  locator: Abstract; 2 Terminology
- url: https://docs.python.org/3/library/zoneinfo.html
  title: zoneinfo, IANA time zone support, The Python Standard Library
  locator: Data sources
- url: https://pkg.go.dev/time
  title: Package time, Go standard library
  locator: func LoadLocation
- url: https://en.cppreference.com/w/cpp/chrono/tzdb
  title: std::chrono::tzdb, cppreference.com
  locator: Description; Member objects
- url: https://learn.microsoft.com/en-us/dotnet/api/system.timezoneinfo
  title: TimeZoneInfo Class, .NET API
  locator: Methods
---

## What it is

The IANA time zone database, also called tz, tzdb or zoneinfo, is the shared record of the local time of the world. The IANA page says that it "contains code and data that represent the history of local time for many representative locations worldwide."[^1] It is "updated periodically to reflect changes made by political bodies to time zone boundaries, UTC offsets, and daylight-saving rules."[^1] The data is in the public domain.

Each zone of the database has a name such as `America/Chicago`, and the zone holds all the offsets and abbreviations that the region has used, with the instants of each change. "Each main entry in the database represents a timezone for a set of civil-time clocks that have all agreed since 1970."[^2] A program that needs the local time of a place asks the database with such a name. See [[time-zone]] for the concept.

Almost all systems use it. The tz page lists among its users the GNU C Library, Android, the BSDs, macOS, iOS, Microsoft Windows and Oracle Database[^2]. Java, Python, Go, .NET, C++ and the JavaScript engines read it or ship a copy of it.

## Syntax or structure

**Names.** A name has the form `Area/Location`. "Timezones are typically identified by continent or ocean and then by the name of the largest city within the region containing the clocks."[^2] The areas are continents, oceans and `Etc`. Underscores replace spaces, as in `America/New_York`[^3]. The names are not tied to countries, so that they stay "robust in the presence of political changes."[^4] When a city name changes, the old name stays as a link: "for example, in 2008 Asia/Calcutta was renamed to Asia/Kolkata due to long-time widespread use of the new city name instead of the old."[^4] Older names such as `US/Central` are links in the file `backward`.

**Source files.** The data is text, in files named after regions such as `northamerica` and `europe`. It has three kinds of line. A `Rule` line states when the clocks change for one or more years. A `Zone` line states an offset and the rules that apply to it, up to an instant. A `Link` line makes one name an alias of another. The Rule lines for Chicago start as follows[^5]:

```text
#Rule NAME    FROM TO   -  IN  ON      AT   SAVE LETTER
Rule  Chicago 1920 only -  Jun 13      2:00 1:00 D
Rule  Chicago 1920 1921 -  Oct lastSun 2:00 0    S
Rule  Chicago 1922 1966 -  Apr lastSun 2:00 1:00 D
```

"The FROM and TO columns, respectively, specify the first and last calendar years defining a contiguous range over which a specific Rule line is to apply."[^5] The column ON accepts a day number, `lastSun` or a form such as `Sun>=8`, which means the first Sunday on or after the 8th. SAVE is the amount added to standard time, and LETTER fills the `%s` in an abbreviation such as `C%sT`.

The Zone lines of Chicago start with its local mean time[^5]:

```text
#Zone NAME            STDOFF    RULES   FORMAT [UNTIL]
Zone  America/Chicago -5:50:36  -       LMT    1883 Nov 18 12:09:24
                      -6:00     US      C%sT   1920
                      -6:00     Chicago C%sT   1936 Mar  1 2:00
                      -5:00     -       EST    1936 Nov 15 2:00
                      -6:00     Chicago C%sT   1942
```

The how-to guide explains that "each line of a Zone represents a steady state, not a transition between states."[^5] Each line holds from the UNTIL of the line before it to its own UNTIL, and the last line has no UNTIL. "The first line typically specifies the mean solar time observed before the introduction of standard time."[^5]

**Compiled files.** The compiler `zic` turns the source into one binary file for each zone. RFC 9636 defines this Time Zone Information Format, TZif: "It is a binary format used by most UNIX systems to calculate local time."[^6] A file starts with the four bytes `TZif` and a version byte. It holds a list of transition instants, a table of local time types (an offset, a DST flag and an abbreviation), and, from version 2, a footer with a POSIX `TZ` string for the instants after the last transition. The version 1 data block has 32-bit transition times, which "are limited to representing time changes from 1901-12-13 20:45:52 through 2038-01-19 03:14:07 UT"[^6], so version 2 and later add a second block with 64-bit times. "The standard path for the timezone database is /usr/share/zoneinfo/ in Linux distributions, macOS, and some other Unix-like systems."[^3]

## Examples

The Python program below reads Rule lines in the source format and computes the date of each change in 1950 and 2024. It then checks each date against the compiled data that `zoneinfo` reads, with the abbreviation at 00:30 and at 03:30 local time on that day.

```python run id=rule-lines
import calendar
from datetime import date, datetime
from zoneinfo import ZoneInfo

# Rule lines in the source format of the tz database (NAME FROM TO - IN ON AT SAVE LETTER).
RULES = """
Rule Chicago 1922 1966 - Apr lastSun 2:00 1:00 D
Rule Chicago 1922 1954 - Sep lastSun 2:00 0    S
Rule Chicago 1955 1966 - Oct lastSun 2:00 0    S
Rule US      2007 max  - Mar Sun>=8  2:00 1:00 D
Rule US      2007 max  - Nov Sun>=1  2:00 0    S
"""
MONTHS = {m: i for i, m in enumerate(calendar.month_abbr) if m}
DAYS = {d: i for i, d in enumerate(calendar.day_abbr)}


def on_day(year, month, on):
    """Returns the date that an ON field such as lastSun, Sun>=8 or 15 names."""
    if on.startswith("last"):
        last = calendar.monthrange(year, month)[1]
        d = date(year, month, last)
        while d.weekday() != DAYS[on[4:]]:
            d = d.replace(day=d.day - 1)
        return d
    if ">=" in on:
        name, low = on.split(">=")
        d = date(year, month, int(low))
        while d.weekday() != DAYS[name]:
            d = d.replace(day=d.day + 1)
        return d
    return date(year, month, int(on))


chicago = ZoneInfo("America/Chicago")
for year in (1950, 2024):
    for line in RULES.strip().splitlines():
        _, name, start, end, _, month, on, at, save, letter = line.split()
        end = 9999 if end == "max" else int(end)
        if not int(start) <= year <= end:
            continue
        d = on_day(year, MONTHS[month], on)
        # Check the date against the compiled data: the abbreviation at 00:30 and at 03:30 local time.
        before = datetime(d.year, d.month, d.day, 0, 30, tzinfo=chicago).tzname()
        after = datetime(d.year, d.month, d.day, 3, 30, tzinfo=chicago).tzname()
        print(name, year, month, on, "->", d, before, "->", after)
```

```output for=rule-lines
Chicago 1950 Apr lastSun -> 1950-04-30 CST -> CDT
Chicago 1950 Sep lastSun -> 1950-09-24 CDT -> CST
US 2024 Mar Sun>=8 -> 2024-03-10 CST -> CDT
US 2024 Nov Sun>=1 -> 2024-11-03 CDT -> CST
```

The example files show one zone at three instants in each language: 1880 with the local mean time -5:50:36, 1936 with EST and no daylight saving time, and 2024 with CDT. They also load the link name `US/Central`. The C# example shows the mapping between IANA names and the zone IDs of Windows.

## Versions

The database has no version numbers in the usual sense. Each release has the name of its year and a letter, and the IANA page shows the latest release, such as "Latest version 2026f Released 2026-10-27"[^1]. "New editions of the database and code are published as changes warrant, usually several times per year."[^3] A release can change the present: the summary of 2026f starts with "Cuba will not fall back 2026-11-01"[^1], which was 5 days after the release.

The TZif format has versions 1 to 4. The format dates from the 1980s, and RFC 9636 obsoletes RFC 8536, the first RFC for it[^6]. Version 2 added the 64-bit data and the footer.

The process has its own rules. RFC 6557 states "procedures involved with maintenance of the TZ database and associated code, including how to submit proposed updates, how decisions for inclusion of those updates are made, and the selection of a designated expert by and for the time zone community."[^7] It defines the TZ Coordinator as "The person or people who maintain and manage release of the TZ Database."[^7]

The project goes back to 1986 or earlier, and its founding contributor was Arthur David Olson[^3]. Paul Eggert has been its editor since 2005. On 30 September 2011 a lawsuit about copyright in the data was filed, and the mailing list and the FTP site were shut down. ICANN took over the maintenance on 14 October 2011, and the lawsuit was resolved on 22 February 2012[^3].

## Pitfalls

- **Old data on the machine.** A program uses the copy of the data that its platform has. Java has its own copy in the JDK, the JavaScript engines have one in ICU, and Python, Go and C read the files of the system. "End users typically receive updates to this data through their software and operating system vendors."[^1] After a government changes its rules, two machines can give different local times for the same instant until both are updated.
- **History before 1970 is partial.** The database records transitions before 1970 for its zones, "because most systems support timestamps before 1970 and could misbehave if data entries were omitted for pre-1970 transitions. However, the database is not designed for and does not suffice for applications requiring accurate handling of all past times everywhere"[^4]. Two cities that share a zone since 1970 can have different clocks before 1970.
- **Names are not stable keys for people.** Names change when city names change, and old names become links. Store the name, but do not show it as the name of a country or compare names as text to decide if two zones are equal. Compare offsets at the instants that matter, or resolve links first.
- **Offsets with seconds.** Local mean time offsets such as -5:50:36 have seconds. A format with `%z`, or a type that stores whole minutes, loses them. The Bash example shows -0550.
- **The 32-bit limit.** A program that reads only the version 1 block of a TZif file gets no transitions after 2038-01-19. Read the version 2 block and the footer. See [[unix-time]] for the year 2038 problem.
- **Windows zone IDs.** Windows has its own set of zone IDs, such as `Central Standard Time`, and fewer zones than the IANA database. A mapping between them "cannot be perfect because the number of time zones in Windows systems is significantly lower than in the IANA TZ database."[^3]
- **Abbreviations are not keys.** The database has abbreviations for display. Many are invented or shared by several zones. Parse zone names, not abbreviations.

## Tools and libraries

- Python: `zoneinfo.ZoneInfo` reads the system files. "By default, zoneinfo uses the system's time zone data if available; if no system time zone data is available, the library will fall back to using the first-party tzdata package available on PyPI."[^8]
- Go: `time.LoadLocation` searches "the directory or uncompressed zip file named by the ZONEINFO environment variable"[^9], then the system, then `$GOROOT/lib/time/zoneinfo.zip`, then the package `time/tzdata` if the program imports it[^9].
- C++: "The class tzdb represents a copy of the IANA time zone database."[^10] `std::chrono::get_tzdb()` returns it, with its `version`, its zones and its links.
- .NET: `TimeZoneInfo.FindSystemTimeZoneById` accepts IANA names, and `TryConvertIanaIdToWindowsId` "Tries to convert an IANA time zone ID to a Windows ID."[^11]
- Java: `java.time` has its own compiled copy of the data, and the JDK tool tzupdater updates it.
- The reference code of the project has the compiler `zic` and the dumper `zdump`. `zdump -v America/Chicago` prints each transition of a zone.

## References
