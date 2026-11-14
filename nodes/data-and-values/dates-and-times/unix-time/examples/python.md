```python run id=seconds-since-the-epoch
import time
from datetime import datetime, timezone

t = 1709985600
# From a Unix time to a date and time in UTC, and back.
print(datetime.fromtimestamp(t, tz=timezone.utc).isoformat())
print(int(datetime(2024, 3, 9, 12, tzinfo=timezone.utc).timestamp()))

# The epoch is the instant 0.
print(time.gmtime(0)[:6])

# A value in milliseconds must be divided by 1000 first.
ms = 1709985600123
print(datetime.fromtimestamp(ms / 1000, tz=timezone.utc).isoformat(timespec="milliseconds"))

# A negative value is an instant before 1970.
print(datetime.fromtimestamp(-1, tz=timezone.utc).isoformat())

# time() gives a float, and time_ns() gives an exact integer.
print(type(time.time()).__name__, type(time.time_ns()).__name__)
```

```output
2024-03-09T12:00:00+00:00
1709985600
(1970, 1, 1, 0, 0, 0)
2024-03-09T12:00:00.123+00:00
1969-12-31T23:59:59+00:00
float int
```

- Verdict: stdlib. time.time gives the current Unix time as a float and time.time_ns as an integer of nanoseconds. datetime.fromtimestamp and datetime.timestamp convert between a Unix time and a datetime, and calendar.timegm converts UTC fields to a Unix time.
