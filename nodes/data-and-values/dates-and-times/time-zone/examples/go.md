```go run id=load-location
package main

import (
	"fmt"
	"time"
)

func main() {
	ny, err := time.LoadLocation("America/New_York")
	if err != nil {
		fmt.Println(err)
		return
	}
	const layout = "2006-01-02T15:04-07:00 MST"
	for _, m := range []time.Month{time.January, time.July} {
		fmt.Println(time.Date(2024, m, 9, 12, 0, 0, 0, time.UTC).In(ny).Format(layout))
	}

	// An overlap: two instants one hour apart have the same local time.
	first := time.Date(2024, 11, 3, 5, 30, 0, 0, time.UTC).In(ny)
	fmt.Println(first.Format(layout), first.Add(time.Hour).Format(layout))

	// A fixed offset with a name is not a zone: its offset never changes.
	est := time.FixedZone("EST", -5*60*60)
	fmt.Println(time.Date(2024, 7, 9, 12, 0, 0, 0, time.UTC).In(est).Format(layout))

	// The name and the offset in seconds of the zone at one instant.
	name, offset := time.Date(2024, 7, 9, 12, 0, 0, 0, ny).Zone()
	fmt.Println(name, offset)
}
```

```output
2024-01-09T07:00-05:00 EST
2024-07-09T08:00-04:00 EDT
2024-11-03T01:30-04:00 EDT 2024-11-03T01:30-05:00 EST
2024-07-09T07:00-05:00 EST
EDT -14400
```

- Verdict: stdlib. time.LoadLocation loads a zone of the IANA database, and time.FixedZone makes a fixed offset. A Time carries its Location, and the method In shows the same instant in another zone.
