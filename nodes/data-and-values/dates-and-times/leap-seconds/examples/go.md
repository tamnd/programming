```go run id=date-normalize
package main

import (
	"fmt"
	"time"
)

func main() {
	// Time has no leap seconds, so Parse rejects the second 60.
	_, err := time.Parse(time.RFC3339, "2016-12-31T23:59:60Z")
	fmt.Println(err)

	// Date normalizes the second 60 to the next minute.
	leap := time.Date(2016, 12, 31, 23, 59, 60, 0, time.UTC)
	fmt.Println(leap.Format(time.RFC3339))

	a := time.Date(2016, 12, 31, 23, 59, 59, 0, time.UTC)
	b := time.Date(2017, 1, 1, 0, 0, 0, 0, time.UTC)
	fmt.Println(b.Sub(a), b.Unix()-a.Unix(), leap.Equal(b))
}
```

```output
parsing time "2016-12-31T23:59:60Z": second out of range
2017-01-01T00:00:00Z
1s 1 true
```

- Verdict: stdlib. The package time assumes "a Gregorian calendar, with no leap seconds". `time.Parse` rejects the second 60, and `time.Date` normalizes it to the next minute. For durations, Go reads the monotonic clock, which a leap second does not step.
