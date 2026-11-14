```bash run id=gnu-date
# GNU date has no second 60.
date -u -d '2016-12-31 23:59:60' >/dev/null 2>&1 || echo "rejected"
for t in 1483228799 1483228800; do date -u -d "@$t" '+%F %T'; done
# %s is Unix time, so the difference across the leap second is 1.
echo $(( $(date -u -d '2017-01-01 00:00:00' +%s) - $(date -u -d '2016-12-31 23:59:59' +%s) ))
```

```output
rejected
2016-12-31 23:59:59
2017-01-01 00:00:00
1
```

- Verdict: idiom. GNU `date` has no second 60 and rejects it. Its `%s` is Unix time, so a difference across a leap second is 1 second short. The tz files under `right/` count leap seconds in `time_t`, but the test server, with Ubuntu 24.04, has none.
