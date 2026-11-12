```go run id=one-band-function
package main

import (
	"fmt"
	"math"
)

func bottomBand(usage int) int { return min(usage, 100) }

func middleBand(usage int) int {
	if usage > 100 {
		return min(usage, 200) - 100
	}
	return 0
}

func topBand(usage int) int {
	if usage > 200 {
		return usage - 200
	}
	return 0
}

// withinBand replaces the three functions above.
func withinBand(usage, bottom, top int) int {
	if usage > bottom {
		return min(usage, top) - bottom
	}
	return 0
}

func main() {
	for _, usage := range []int{0, 50, 100, 150, 200, 350} {
		if withinBand(usage, 0, 100) != bottomBand(usage) ||
			withinBand(usage, 100, 200) != middleBand(usage) ||
			withinBand(usage, 200, math.MaxInt) != topBand(usage) {
			panic(fmt.Sprint("mismatch at ", usage))
		}
	}
	fmt.Println("parameterize function: tests pass")
}
```

```output
parameterize function: tests pass
```
