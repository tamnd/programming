```go run id=time-time
package main

import (
	"fmt"
	"strings"
	"time"
)

func main() {
	// A time.Time is an instant with a location for display.
	t := time.Date(2024, time.March, 9, 12, 0, 0, 0, time.UTC)
	fmt.Println(t.Format(time.RFC3339), t.Unix())

	// The same instant in another location. Equal compares instants, and == compares fields.
	tokyo := t.In(time.FixedZone("JST", 9*60*60))
	fmt.Println(tokyo.Format(time.RFC3339), t.Equal(tokyo), t == tokyo)

	// AddDate normalizes: January 31 plus 1 month is February 31, which is March 2.
	jan31 := time.Date(2024, time.January, 31, 0, 0, 0, 0, time.UTC)
	fmt.Println(jan31.AddDate(0, 1, 0).Format(time.DateOnly))

	// A time.Duration is a count of nanoseconds.
	d := 90 * time.Minute
	fmt.Println(d, d.Seconds())

	// time.Now has a monotonic clock reading. Round(0) removes it.
	now := time.Now()
	fmt.Println(strings.Contains(now.String(), "m="), strings.Contains(now.Round(0).String(), "m="))
}
```

```output
2024-03-09T12:00:00Z 1709985600
2024-03-09T21:00:00+09:00 true false
2024-03-02
1h30m0s 5400
true false
```

- Verdict: stdlib. The package time has Time, Duration and Location. A Time is an instant with a location, and the Time from time.Now also has a monotonic clock reading for elapsed time. There is no separate date type: a date is a Time at midnight.
