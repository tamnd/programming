---
tag: NEW-python-naive-datetimes
kind: feature
owner: python
slug: naive-datetimes
name: Naive datetimes
aliases: [naive datetime, aware datetime, datetime.utcnow, can't compare offset-naive and offset-aware datetimes, timezone.utc, zoneinfo, DTZ001, DTZ003, DTZ005]
area: languages/python/pitfalls
level: core
summary: >-
  A datetime with no tzinfo is naive: it does not say which zone it is in, and many methods treat it as local time. Use aware values, such as datetime.now(timezone.utc), and zoneinfo for local zones.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/datetime.html#aware-and-naive-objects", locator: "datetime, Aware and naive objects"}
proposals: []
see_also: [python/dates-and-times, python/time-and-clocks, python/warnings, python/comparison-operators, python/saving-structured-data-with-json, python/the-standard-library-tour]
sources:
  - url: https://docs.python.org/3.15/library/datetime.html#aware-and-naive-objects
    title: datetime, Basic date and time types, Python 3.15 documentation
    locator: Aware and naive objects; datetime.utcnow(); datetime.timestamp(); Supported operations
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.12.html#deprecated
    title: What’s New In Python 3.12
    locator: Deprecated, datetime
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/zoneinfo.html
    title: zoneinfo, IANA time zone support, Python 3.15 documentation
    locator: Introduction
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/call-datetime-now-without-tzinfo/
    title: call-datetime-now-without-tzinfo (DTZ005), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/call-datetime-utcnow/
    title: call-datetime-utcnow (DTZ003), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/call-datetime-without-tzinfo/
    title: call-datetime-without-tzinfo (DTZ001), Ruff documentation
    locator: What it does
    accessed: 2026-11-18
---

## Synopsis

```python fails id=mixed-compare expect=exception:TypeError
from datetime import datetime, timezone

deadline = datetime(2026, 11, 18, 12, 0)
now = datetime(2026, 11, 18, 11, 0, tzinfo=timezone.utc)

print(deadline == now)
print(now < deadline)
```

The first `datetime` is naive and the second is aware. The `==` test gives `False`, and the `<` test raises `TypeError`, because Python cannot know which zone the naive value is in.

## Semantics

**r1.** "Date and time objects may be categorized as “aware” or “naive” depending on whether or not they include time zone information."[^1] "A naive object does not contain enough information to unambiguously locate itself relative to other date/time objects. Whether a naive object represents Coordinated Universal Time (UTC), local time, or time in some other time zone is purely up to the program".[^1]

**r2.** Naive and aware values do not mix: "Naive and aware datetime objects are never equal." and "Order comparison between naive and aware datetime objects raises TypeError."[^1] Example: [`mixed-compare`](#mixed-compare).

**r3.** Many methods read a naive value as local time. For `timestamp`, "Naive datetime instances are assumed to represent local time".[^1] So a naive value that holds UTC gives a wrong instant on a computer whose zone is not UTC. Example: [`utcnow-timestamp`](#utcnow-timestamp).

**r4.** The documentation gives the fix: "Because naive datetime objects are treated by many datetime methods as local times, it is preferred to use aware datetimes to represent times in UTC. As such, the recommended way to create an object representing the current time in UTC is by calling datetime.now(timezone.utc)."[^1] Example: [`aware-now`](#aware-now).

**r5.** For a named zone, use `zoneinfo`: "The zoneinfo module provides a concrete time zone implementation to support the IANA time zone database".[^3] Store and compute in UTC, and convert to a local zone for display. Example: [`zoneinfo-dst`](#zoneinfo-dst).

**r6.** Arithmetic in one zone ignores offset changes. "If both are naive, or both are aware and have the same tzinfo attribute, the tzinfo attributes are ignored, and the result is a timedelta object t such that datetime2 + t == datetime1. No time zone adjustments are done in this case."[^1] So a day in a local zone can be 23 or 25 hours long, and the subtraction still says one day. Example: [`same-zone-math`](#same-zone-math).

**r7.** The linters:

- Ruff DTZ005 "Checks for usages of datetime.datetime.now() that do not specify a timezone."[^4]
- Ruff DTZ003 "Checks for usage of datetime.datetime.utcnow()."[^5] Its reason: "datetime.datetime.utcnow() returns a naive datetime object; instead, use datetime.datetime.now(tz=...) to create a timezone-aware object."[^5]
- Ruff DTZ001 "Checks for datetime instantiations that do not specify a timezone."[^6]

## Examples

```python run id=aware-now
from datetime import UTC, datetime, timezone

print(datetime.now().tzinfo)
print(datetime.now(timezone.utc).tzinfo)
print(UTC is timezone.utc)

stamp = datetime(2026, 11, 18, 9, 30, tzinfo=UTC)
print(stamp.isoformat())
```

```output for=aware-now
None
UTC
True
2026-11-18T09:30:00+00:00
```

`datetime.now()` with no argument is naive. With a zone, it is aware, and `isoformat` writes the offset, so the text is clear to any reader (rule r4).

```python run id=zoneinfo-dst
from datetime import UTC, datetime, timedelta
from zoneinfo import ZoneInfo

new_york = ZoneInfo("America/New_York")
start = datetime(2026, 3, 8, 6, 30, tzinfo=UTC)
for hours in [0, 1]:
    moment = start + timedelta(hours=hours)
    print(moment.astimezone(new_york))
```

```output for=zoneinfo-dst
2026-03-08 01:30:00-05:00
2026-03-08 03:30:00-04:00
```

One hour later in UTC is two hours later on the clock in New York, because daylight saving time starts in between. `zoneinfo` knows the rules of the zone (rule r5).

## Versions

> [!since v="3.11"]
> `datetime.UTC` is an "Alias for the UTC time zone singleton datetime.timezone.utc."[^1]

> [!since v="3.12"]
> "datetime.datetime’s utcnow() and utcfromtimestamp() are deprecated and will be removed in a future version. Instead, use timezone-aware objects to represent datetimes in UTC: respectively, call now() and fromtimestamp() with the tz parameter set to datetime.UTC."[^2] Example: [`utcnow-warns`](#utcnow-warns).

## Pitfalls

```python wrong id=utcnow-timestamp
import subprocess
import sys

code = (
    "import time, warnings\n"
    "from datetime import datetime, timezone\n"
    "warnings.simplefilter('ignore')\n"
    "naive = datetime.utcnow().timestamp() - time.time()\n"
    "aware = datetime.now(timezone.utc).timestamp() - time.time()\n"
    "print(round(naive / 3600), round(aware / 3600))\n"
)
result = subprocess.run(
    [sys.executable, "-c", code],
    env={"TZ": "Asia/Ho_Chi_Minh"}, capture_output=True, text=True,
)
print(result.stdout.strip())
```

```output for=utcnow-timestamp
-7 0
```

In a zone at UTC+7, the naive value from `utcnow` was read as local time, so its timestamp is 7 hours off (rule r3). The aware value gives the right instant.

```python wrong id=same-zone-math
from datetime import UTC, datetime
from zoneinfo import ZoneInfo

new_york = ZoneInfo("America/New_York")
start = datetime(2026, 3, 7, 12, 0, tzinfo=new_york)
end = datetime(2026, 3, 8, 12, 0, tzinfo=new_york)

print(end - start)
print(end.astimezone(UTC) - start.astimezone(UTC))
```

```output for=same-zone-math
1 day, 0:00:00
23:00:00
```

Both values have the same `tzinfo`, so the first subtraction ignores the change of offset (rule r6). The time that really passed is 23 hours. Subtract in UTC to get it.

```python run id=utcnow-warns
import warnings
from datetime import datetime

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    datetime.utcnow()
print(caught[0].category.__name__)
```

```output for=utcnow-warns
DeprecationWarning
```

From Python 3.12, `utcnow` emits a `DeprecationWarning`.[^2]

## Comparison

In Python, one `datetime` type holds both naive and aware values, and the type does not show which kind a value is (rule r1). So the bug shows only at run time, in a comparison or a conversion. Make all values aware at the edges of the program, where data comes in. For dates and times in general, see [[python/dates-and-times]]. For clocks and timestamps, see [[python/time-and-clocks]].
