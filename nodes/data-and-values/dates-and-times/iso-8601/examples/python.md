```python run id=fromisoformat
from datetime import date, datetime, timezone

# fromisoformat reads most ISO 8601 forms, but not reduced precision, ordinal dates or durations.
for s in ["2024-07-09", "2024-07-09 12:30", "2024-07-09T12:30:05.5+05:30", "2024-07", "2024-191", "P1DT2H"]:
    try:
        print(f"{s:28} {datetime.fromisoformat(s).isoformat()}")
    except ValueError:
        print(f"{s:28} ValueError")

t = datetime(2024, 7, 9, 12, 30, 5, 120000, tzinfo=timezone.utc)
print(t.isoformat())
print(t.isoformat(timespec="milliseconds"))
print(date.fromisocalendar(2025, 1, 1), date(2024, 12, 30).isocalendar())
```

```output
2024-07-09                   2024-07-09T00:00:00
2024-07-09 12:30             2024-07-09T12:30:00
2024-07-09T12:30:05.5+05:30  2024-07-09T12:30:05.500000+05:30
2024-07                      ValueError
2024-191                     ValueError
P1DT2H                       ValueError
2024-07-09T12:30:05.120000+00:00
2024-07-09T12:30:05.120+00:00
2024-12-30 datetime.IsoCalendarDate(year=2025, week=1, weekday=1)
```

- Verdict: stdlib. `datetime.isoformat()` writes the extended format, and `fromisoformat()` reads most ISO 8601 forms, including the basic format and week dates. It rejects reduced precision such as `2024-07`, ordinal dates and durations. It writes `+00:00`, never `Z`, for UTC. `isocalendar()` and `fromisocalendar()` convert week dates.
