```go run id=special-cases-first
package main

import (
	"errors"
	"fmt"
	"strconv"
	"strings"
)

// parseSeatBefore nests the normal path inside two checks.
func parseSeatBefore(s string) (int, error) {
	if s != "" {
		n, err := strconv.Atoi(strings.TrimPrefix(s, "S"))
		if err == nil {
			if n >= 1 && n <= 40 {
				return n, nil
			} else {
				return 0, errors.New("no such seat")
			}
		} else {
			return 0, err
		}
	} else {
		return 0, errors.New("empty seat")
	}
}

// parseSeat checks each error case first and returns.
func parseSeat(s string) (int, error) {
	if s == "" {
		return 0, errors.New("empty seat")
	}
	n, err := strconv.Atoi(strings.TrimPrefix(s, "S"))
	if err != nil {
		return 0, err
	}
	if n < 1 || n > 40 {
		return 0, errors.New("no such seat")
	}
	return n, nil
}

func main() {
	for _, s := range []string{"", "S7", "S41", "S0", "Sx", "12"} {
		a, errA := parseSeat(s)
		b, errB := parseSeatBefore(s)
		if a != b || (errA == nil) != (errB == nil) || (errA != nil && errA.Error() != errB.Error()) {
			panic(fmt.Sprint("differs at ", s))
		}
	}
	fmt.Println("guard clauses: tests pass")
}
```

```output
guard clauses: tests pass
```
