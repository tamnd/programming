```python run id=aware-naive-timedelta
from datetime import date, datetime, timedelta, timezone

# An aware datetime has a zone, so it is an instant.
instant = datetime(2024, 3, 9, 12, 0, tzinfo=timezone.utc)
# A naive datetime has no zone. It is a reading of a wall clock.
wall = datetime(2024, 3, 9, 12, 0)
print(instant.tzinfo, wall.tzinfo)

# Equality between the two kinds is False, and an order test raises TypeError.
print(instant == wall)
try:
    instant < wall
except TypeError as e:
    print(type(e).__name__)

# The same instant at another offset is equal.
tokyo = instant.astimezone(timezone(timedelta(hours=9)))
print(tokyo.isoformat(), tokyo == instant)
print(instant.timestamp())

# A date has no time and no zone. 2024 is a leap year.
print(date(2024, 2, 28) + timedelta(days=1))

# A timedelta is an exact amount of time.
print(timedelta(hours=36), timedelta(minutes=90).total_seconds())
```

```output
UTC None
False
TypeError
2024-03-09T21:00:00+09:00 True
1709985600.0
2024-02-29
1 day, 12:00:00 5400.0
```

- Verdict: stdlib. The module datetime has date, time, datetime and timedelta. A datetime with a tzinfo is aware and names an instant. A datetime without one is naive, and the program decides what it means. The module zoneinfo gives the zones of the IANA database.
