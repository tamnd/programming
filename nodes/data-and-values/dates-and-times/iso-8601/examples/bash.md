```bash run id=date-iso
export TZ=UTC
# printf has the strftime conversions: %F is %Y-%m-%d and %T is %H:%M:%S.
printf '%(%FT%TZ)T\n' 1735560000
printf '%(%G-W%V-%u %Y-%j)T\n' 1735560000

# GNU date reads ISO 8601 text with an offset and writes the extended format with -I.
date -u -d '2024-07-09T14:30:05+02:00' -Iseconds
date -u -d '2024-07-09T14:30:05.12+02:00' -Ins
TZ=Asia/Kolkata date -d @1720528205 -Iseconds
```

```output
2024-12-30T12:00:00Z
2025-W01-1 2024-365
2024-07-09T12:30:05+00:00
2024-07-09T12:30:05,120000000+00:00
2024-07-09T18:00:05+05:30
```

- Verdict: idiom. Bash has no date type. `printf '%(...)T'` formats a count of seconds with the conversions of `strftime`, and GNU `date` parses ISO 8601 text and writes it with the option `-I`. Note the comma in the fraction of `-Ins`: ISO 8601 allows a comma or a full stop there, but RFC 3339 allows only the full stop.
