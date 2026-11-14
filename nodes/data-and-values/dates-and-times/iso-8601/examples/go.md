```go run id=rfc3339-layout
package main

import (
	"fmt"
	"time"
)

func main() {
	t := time.Date(2024, 7, 9, 14, 30, 5, 120_000_000, time.FixedZone("", 2*60*60))
	fmt.Println(t.Format(time.RFC3339))
	fmt.Println(t.Format(time.RFC3339Nano)) // trailing zeros of the fraction are dropped
	fmt.Println(t.UTC().Format(time.RFC3339Nano))
	fmt.Println(t.Format(time.DateOnly))

	// ISOWeek gives the week-numbering year and the week. Go counts Sunday as 0, ISO 8601 counts it as 7.
	d := time.Date(2024, 12, 30, 0, 0, 0, 0, time.UTC)
	year, week := d.ISOWeek()
	fmt.Printf("%d-W%02d-%d\n", year, week, (int(d.Weekday())+6)%7+1)

	// time.RFC3339 as a layout for Parse accepts a fraction, but it needs the T and an offset.
	for _, s := range []string{"2024-07-09T12:30:05Z", "2024-07-09T18:00:05.12+05:30", "2024-07-09 12:30:05Z", "2024-07-09T12:30:05"} {
		u, err := time.Parse(time.RFC3339, s)
		if err != nil {
			fmt.Println(s, "error")
			continue
		}
		fmt.Println(s, u.UTC().Format(time.RFC3339Nano))
	}
}
```

```output
2024-07-09T14:30:05+02:00
2024-07-09T14:30:05.12+02:00
2024-07-09T12:30:05.12Z
2024-07-09
2025-W01-1
2024-07-09T12:30:05Z 2024-07-09T12:30:05Z
2024-07-09T18:00:05.12+05:30 2024-07-09T12:30:05.12Z
2024-07-09 12:30:05Z error
2024-07-09T12:30:05 error
```

- Verdict: stdlib. The package `time` has the layouts `time.RFC3339` and `time.RFC3339Nano` for the profile of RFC 3339, and `ISOWeek` for week numbers. It has no parser for week dates, ordinal dates or durations, and `time.ParseDuration` uses its own form such as `1h30m`, not `PT1H30M`.
