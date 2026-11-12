```go run id=duration-and-named-types
package main

import (
	"fmt"
	"time"
)

type Metres float64
type Feet float64

func (f Feet) Metres() Metres { return Metres(f * 0.3048) }

func climb(altitude, by Metres) Metres { return altitude + by }

func main() {
	timeout := 90*time.Second + 500*time.Millisecond
	fmt.Println(timeout, "=", timeout.Milliseconds(), "ms")
	fmt.Println("2m / 30s as Duration:", (2*time.Minute)/(30*time.Second))
	fmt.Println("2m / 30s as a number:", (2*time.Minute).Seconds()/(30*time.Second).Seconds())
	retries := 3
	fmt.Println("total wait:", time.Duration(retries)*timeout)
	fmt.Println("climb:", climb(1000, Feet(500).Metres()))
}
```

```output for=duration-and-named-types
1m30.5s = 90500 ms
2m / 30s as Duration: 4ns
2m / 30s as a number: 4
total wait: 4m31.5s
climb: 1152.4
```

```go fails expect=compile id=int-times-duration
package main

import (
	"fmt"
	"time"
)

func main() {
	retries := 3
	timeout := 2 * time.Second
	fmt.Println(retries * timeout)
}
```

- Verdict: idiom. `time.Duration` is a named type for an `int64` count of nanoseconds, and the constants such as `time.Second` give it units. The fails block multiplies an `int` variable by a `Duration`, and the compiler rejects the mismatched types; the fix is `time.Duration(retries) * timeout`. The type does not track dimensions: a `Duration` divided by a `Duration` is still a `Duration`, so the run block prints `4ns` where the answer is the number 4. Named types such as `Metres` and `Feet` keep units apart in the same way.
