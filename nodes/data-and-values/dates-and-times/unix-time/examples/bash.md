```bash run id=epochseconds
export TZ=UTC
t=1709985600

# The printf format %(...)T formats a Unix time with strftime.
printf '%(%Y-%m-%dT%H:%M:%SZ)T\n' "$t"
printf '%(%Y-%m-%d)T\n' 0

# Integer arithmetic gives the days since the epoch and the second of the day.
echo $((t / 86400)) $((t % 86400))

# EPOCHSECONDS is the current Unix time.
if ((EPOCHSECONDS > 1700000000)); then echo after-2023; fi
```

```output
2024-03-09T12:00:00Z
1970-01-01
19791 43200
after-2023
```

- Verdict: idiom. Bash has no time type, but the variables EPOCHSECONDS and EPOCHREALTIME give the current Unix time, and the format %(datefmt)T of printf formats one. The external command date prints a Unix time with date +%s.
