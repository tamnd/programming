```go run id=duration
package main

import (
	"fmt"
	"regexp"
	"strconv"
)

// Duration has an unexported field, so other packages build it only
// through the functions below.
type Duration struct {
	seconds int64
}

var zero = &Duration{0}

func OfSeconds(seconds int64) *Duration {
	if seconds == 0 {
		return zero // the same instance every time
	}
	return &Duration{seconds}
}

func OfMinutes(minutes int64) *Duration {
	return OfSeconds(minutes * 60)
}

var text = regexp.MustCompile(`^(\d+)([sm])$`)

func Parse(s string) (*Duration, error) {
	m := text.FindStringSubmatch(s)
	if m == nil {
		return nil, fmt.Errorf("bad duration %q", s)
	}
	n, _ := strconv.ParseInt(m[1], 10, 64)
	if m[2] == "m" {
		n *= 60
	}
	return OfSeconds(n), nil
}

func (d *Duration) String() string { return fmt.Sprintf("%d s", d.seconds) }

func main() {
	fmt.Println("OfMinutes(2) =", OfMinutes(2))
	fmt.Println("OfSeconds(90) =", OfSeconds(90))
	fmt.Println("zero is cached:", OfSeconds(0) == OfMinutes(0))
	d, err := Parse("2m")
	fmt.Println(`Parse("2m") =`, d, err)
	d, err = Parse("2h")
	fmt.Println(`Parse("2h") =`, d, err)
}
```

```output
OfMinutes(2) = 120 s
OfSeconds(90) = 90 s
zero is cached: true
Parse("2m") = 120 s <nil>
Parse("2h") = <nil> bad duration "2h"
```

- Verdict: idiom. Go has no constructors. A package-level function, by convention named `New` or `NewT`, builds the value, as `ring.New` and `os.NewFile` do.
- The function is not a method of the type, so the name says the type: `OfSeconds` here, or `time.ParseDuration` in the standard library.
- A Go factory that can fail returns a second result of type `error`.
