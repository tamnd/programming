```python run id=zone-history
from datetime import datetime, timezone
from zoneinfo import ZoneInfo, available_timezones

chicago = ZoneInfo("America/Chicago")
# Three instants in the history of one zone: local mean time, a year of EST, and today's rules.
for when in (datetime(1880, 1, 1, 12), datetime(1936, 6, 1, 12), datetime(2024, 7, 1, 12)):
    t = when.replace(tzinfo=timezone.utc).astimezone(chicago)
    print(t.isoformat(), t.tzname())

# US/Central is a link (an alias) in the file backward. It gives the same offsets.
old = ZoneInfo("US/Central")
t = datetime(2024, 7, 1, 12, tzinfo=timezone.utc)
print(old.key, t.astimezone(old).utcoffset() == t.astimezone(chicago).utcoffset())
print("America/Chicago" in available_timezones())
```

```output
1880-01-01T06:09:24-05:50:36 LMT
1936-06-01T07:00:00-05:00 EST
2024-07-01T07:00:00-05:00 CDT
US/Central True
True
```

- Verdict: stdlib. `zoneinfo.ZoneInfo` reads the compiled files of the system, or the package tzdata from PyPI. One zone gives local mean time before 1883, EST in 1936 and CDT today, because the file holds the whole history. A link name such as `US/Central` loads the same rules under its own key.
