```python run id=zoneinfo-gap-and-fold
from datetime import datetime, timedelta, timezone
from zoneinfo import ZoneInfo

ny = ZoneInfo("America/New_York")

# A gap: on 2024-03-10 the clocks in New York jump from 02:00 to 03:00.
gap = datetime(2024, 3, 10, 2, 30, tzinfo=ny)
print(gap.astimezone(timezone.utc).astimezone(ny).isoformat())

# An overlap: on 2024-11-03, 01:30 happens two times. fold picks one of them.
for fold in (0, 1):
    t = datetime(2024, 11, 3, 1, 30, fold=fold, tzinfo=ny)
    print(t.isoformat(), t.tzname(), t.astimezone(timezone.utc).strftime("%H:%M"))

# A zone gives the offset for each instant. A fixed offset does not change.
fixed = timezone(timedelta(hours=-5))
july = datetime(2024, 7, 9, 12, tzinfo=timezone.utc)
print(july.astimezone(ny).isoformat(), july.astimezone(fixed).isoformat())
```

```output
2024-03-10T03:30:00-04:00
2024-11-03T01:30:00-04:00 EDT 05:30
2024-11-03T01:30:00-05:00 EST 06:30
2024-07-09T08:00:00-04:00 2024-07-09T07:00:00-05:00
```

- Verdict: stdlib. zoneinfo.ZoneInfo gives a zone of the IANA database as a tzinfo, and datetime.timezone gives a fixed offset. The attribute fold chooses between the two readings of a local time in an overlap.
