```python run id=wall-clock-days
from datetime import datetime, timedelta, timezone
from zoneinfo import ZoneInfo

ny = ZoneInfo("America/New_York")
# 2024-11-03 has 25 hours in New York: the clocks go back at 02:00.
a = datetime(2024, 11, 2, 18, 0, tzinfo=ny)
b = a + timedelta(days=1)
print(b, b.tzname())

# With the same tzinfo, subtraction ignores the offsets and gives wall-clock time.
print(b - a)
# Convert to UTC to get the elapsed time.
print(b.astimezone(timezone.utc) - a.astimezone(timezone.utc))

# Exact arithmetic: add 24 hours to the instant, then show it in New York.
c = (a.astimezone(timezone.utc) + timedelta(hours=24)).astimezone(ny)
print(c, c.tzname())
```

```output
2024-11-03 18:00:00-05:00 EST
1 day, 0:00:00
1 day, 1:00:00
2024-11-03 17:00:00-05:00 EST
```

- Verdict: stdlib. A `timedelta` has days, seconds and microseconds but no months, so a program adds months with `calendar.monthrange`, as the Idea block does. An aware `datetime` plus a `timedelta` keeps the wall-clock time, and the difference of two values with the same `tzinfo` ignores the offsets. Convert to UTC to get exact durations.
