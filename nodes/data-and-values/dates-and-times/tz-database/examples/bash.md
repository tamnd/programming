```bash run id=tz-history
# A zone name in TZ makes the C library read the compiled file of the zone.
in_zone() (
  export TZ=$1
  printf '%(%Y-%m-%dT%H:%M:%S%z %Z)T\n' "$2"
)

# 1880-01-01, 1936-06-01 and 2024-07-01, each at 12:00 UTC.
for t in -2840097600 -1059825600 1719835200; do
  in_zone America/Chicago "$t"
done
in_zone US/Central 1719835200
# The compiled files are in a directory such as /usr/share/zoneinfo.
[ -f /usr/share/zoneinfo/America/Chicago ] && echo "file found"
```

```output
1880-01-01T06:09:24-0550 LMT
1936-06-01T07:00:00-0500 EST
2024-07-01T07:00:00-0500 CDT
2024-07-01T07:00:00-0500 CDT
file found
```

- Verdict: idiom. Bash has no zone type. A zone name in the variable `TZ` selects a compiled file of the tz database for `printf '%(...)T'` and for the command `date`. The offset -0550 drops the 36 seconds of the real offset, because `%z` has no field for seconds.
