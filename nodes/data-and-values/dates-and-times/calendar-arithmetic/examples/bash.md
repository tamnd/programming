```bash run id=gnu-date-relative
# GNU date adds a month by the field, then normalizes the overflow.
date -u -d '2024-01-31 +1 month' +%F
date -u -d '2024-03-31 -1 month' +%F

# 1 day keeps the local time; 24 hours adds an exact duration.
export TZ=America/New_York
date -d '2024-03-09 18:00 1 day' '+%F %R %Z'
date -d '2024-03-09 18:00 24 hours' '+%F %R %Z'

# After a time of day, +1 is read as the UTC offset +01:00, not as a number of days.
date -d '2024-03-09 18:00 +1 day' '+%F %R %Z'
```

```output
2024-03-02
2024-03-02
2024-03-10 18:00 EDT
2024-03-10 19:00 EDT
2024-03-10 13:00 EDT
```

- Verdict: idiom. GNU `date -d` reads relative items such as `1 month` and `24 hours`. It adds a month by the field and then normalizes, so January 31 plus 1 month is March 2. A sign after a time of day is read as a UTC offset, so put the relative item without a sign or before the date.
