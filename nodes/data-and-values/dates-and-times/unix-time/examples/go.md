```go run id=time-unix
package main

import (
	"fmt"
	"time"
)

func main() {
	// time.Unix makes a Time from seconds and nanoseconds since the epoch.
	t := time.Unix(1709985600, 0).UTC()
	fmt.Println(t.Format(time.RFC3339))
	fmt.Println(t.Unix(), t.UnixMilli())

	// The Unix time does not depend on the location of the Time.
	tokyo := time.FixedZone("UTC+9", 9*60*60)
	fmt.Println(t.In(tokyo).Unix() == t.Unix())

	fmt.Println(time.UnixMilli(1709985600123).UTC().Format(time.RFC3339Nano))

	// A signed 32-bit count wraps to 1901 one second after its maximum.
	var small int32 = 2147483647
	small++
	fmt.Println(small, time.Unix(int64(small), 0).UTC().Format(time.RFC3339))
}
```

```output
2024-03-09T12:00:00Z
1709985600 1709985600000
true
2024-03-09T12:00:00.123Z
-2147483648 1901-12-13T20:45:52Z
```

- Verdict: stdlib. time.Unix, time.UnixMilli and time.UnixMicro make a Time from a Unix time, and the methods Unix, UnixMilli, UnixMicro and UnixNano give it back as an int64.
