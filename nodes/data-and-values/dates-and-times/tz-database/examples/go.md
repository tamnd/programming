```go run id=load-location-history
package main

import (
	"fmt"
	"time"
)

func main() {
	// LoadLocation reads the zone from the tz data of the system, or from time/tzdata if the program embeds it.
	chicago, err := time.LoadLocation("America/Chicago")
	if err != nil {
		fmt.Println("error:", err)
		return
	}
	for _, s := range []string{"1880-01-01T12:00:00Z", "1936-06-01T12:00:00Z", "2024-07-01T12:00:00Z"} {
		t, _ := time.Parse(time.RFC3339, s)
		fmt.Println(t.In(chicago).Format("2006-01-02T15:04:05-07:00:00 MST"))
	}
	// Europe/Vatican is a link to Europe/Rome. The Location keeps the name it was loaded with.
	vatican, _ := time.LoadLocation("Europe/Vatican")
	rome, _ := time.LoadLocation("Europe/Rome")
	t := time.Date(2024, 7, 1, 12, 0, 0, 0, time.UTC)
	_, a := t.In(vatican).Zone()
	_, b := t.In(rome).Zone()
	fmt.Println(vatican, a == b)
}
```

```output
1880-01-01T06:09:24-05:50:36 LMT
1936-06-01T07:00:00-05:00:00 EST
2024-07-01T07:00:00-05:00:00 CDT
Europe/Vatican true
```

- Verdict: stdlib. `time.LoadLocation` reads the compiled file of the zone from the system. A program that imports the package `time/tzdata` embeds a copy of the data, so it also runs on a system with no zone files. The `Location` keeps the name it was loaded with, so a link prints its own name.
