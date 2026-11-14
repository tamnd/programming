```go run id=add-date
package main

import (
	"fmt"
	"time"
)

func main() {
	// AddDate normalizes: January 31 + 1 month is February 31, which is March 2 in 2024.
	jan31 := time.Date(2024, 1, 31, 0, 0, 0, 0, time.UTC)
	fmt.Println(jan31.AddDate(0, 1, 0).Format(time.DateOnly))
	fmt.Println(time.Date(2024, 2, 29, 0, 0, 0, 0, time.UTC).AddDate(1, 0, 0).Format(time.DateOnly))

	// Clamp by hand: day 0 of the month after is the last day of the month.
	last := time.Date(2024, 3, 0, 0, 0, 0, 0, time.UTC).Day()
	fmt.Println(time.Date(2024, 2, min(31, last), 0, 0, 0, 0, time.UTC).Format(time.DateOnly))

	// AddDate keeps the wall clock; Add adds an exact duration.
	ny, err := time.LoadLocation("America/New_York")
	if err != nil {
		panic(err)
	}
	t := time.Date(2024, 3, 9, 18, 0, 0, 0, ny)
	day := t.AddDate(0, 0, 1)
	fmt.Println(day.Format(time.RFC3339), day.Sub(t))
	fmt.Println(t.Add(24 * time.Hour).Format(time.RFC3339))
}
```

```output
2024-03-02
2025-03-01
2024-02-29
2024-03-10T18:00:00-04:00 23h0m0s
2024-03-10T19:00:00-04:00
```

- Verdict: stdlib. `AddDate` adds years, months and days and then normalizes, so January 31 plus 1 month is March 2. It does not clamp. In a `Location`, `AddDate` keeps the wall clock, and `Add` adds an exact `Duration`.
