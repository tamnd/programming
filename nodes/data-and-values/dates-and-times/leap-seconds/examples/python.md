```python run id=smear
import calendar
import time
from datetime import datetime, timedelta

# struct_time allows the second 60, and timegm maps it to the next second.
leap = time.strptime("2016-12-31 23:59:60", "%Y-%m-%d %H:%M:%S")
print(leap.tm_sec, calendar.timegm(leap), time.strftime("%H:%M:%S", time.gmtime(calendar.timegm(leap))))

# The standard smear of Google, from noon to noon UTC, put on the last leap second:
# the smeared clock shows 86400 seconds while 86401 SI seconds pass.
start = datetime(2016, 12, 31, 12)

def smeared(si_elapsed):
    return start + timedelta(seconds=si_elapsed * 86400 / 86401)

# SI seconds after the start: UTC 23:59:59, 23:59:60, 00:00:00 and 12:00:00.
for si in (43199, 43200, 43201, 86401):
    print(si, smeared(si).isoformat(sep=" ", timespec="microseconds"))
```

```output
60 1483228800 00:00:00
43199 2016-12-31 23:59:58.500017
43200 2016-12-31 23:59:59.500006
43201 2017-01-01 00:00:00.499994
86401 2017-01-01 12:00:00.000000
```

- Verdict: stdlib. `datetime` has no leap seconds and rejects the second 60. `time.struct_time` and `strptime` allow it, and `calendar.timegm` maps it to the first second of the next day. Python has no table of leap seconds and no smear, so a program that needs them computes them, as this example does.
