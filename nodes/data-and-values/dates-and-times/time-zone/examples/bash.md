```bash run id=tz-variable
# The variable TZ sets the zone for the format %(...)T of printf.
in_zone() (
  export TZ=$1
  printf '%(%Y-%m-%dT%H:%M%z %Z)T\n' "$2"
)

in_zone America/New_York 1704801600
in_zone America/New_York 1720526400
in_zone Asia/Kolkata 1704801600
in_zone UTC 1704801600
```

```output
2024-01-09T07:00-0500 EST
2024-07-09T08:00-0400 EDT
2024-01-09T17:30+0530 IST
2024-01-09T12:00+0000 UTC
```

- Verdict: idiom. Bash has no zone type. The variable TZ sets the zone of a command or a subshell, and the format %(datefmt)T of printf and the external command date use it.
