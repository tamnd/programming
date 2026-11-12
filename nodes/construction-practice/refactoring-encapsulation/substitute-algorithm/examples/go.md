```go run id=same-answers
package main

import (
	"fmt"
	"time"
)

// Old body: the rule written out by hand.
func isLeapOld(y int) bool {
	switch {
	case y%400 == 0:
		return true
	case y%100 == 0:
		return false
	default:
		return y%4 == 0
	}
}

// Replacement: time.Date normalizes February 29 to March 1
// in a year that has no leap day.
func isLeap(y int) bool {
	return time.Date(y, time.February, 29, 0, 0, 0, 0, time.UTC).Month() == time.February
}

func main() {
	for y := 1000; y <= 3000; y++ {
		if isLeap(y) != isLeapOld(y) {
			panic(fmt.Sprint("differ at ", y))
		}
	}
	if isLeap(1900) || !isLeap(2000) {
		panic("edge years")
	}
	fmt.Println("substitute algorithm: tests pass")
}
```

```output
substitute algorithm: tests pass
```
