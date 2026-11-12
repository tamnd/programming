```go run id=returned-value
package main

import "fmt"

// calculateAscentBefore adds to a total that the caller owns.
func calculateAscentBefore(elevations []int, total *int) {
	for i := 1; i < len(elevations); i++ {
		if change := elevations[i] - elevations[i-1]; change > 0 {
			*total += change
		}
	}
}

func calculateAscent(elevations []int) int {
	result := 0
	for i := 1; i < len(elevations); i++ {
		if change := elevations[i] - elevations[i-1]; change > 0 {
			result += change
		}
	}
	return result
}

func main() {
	points := []int{10, 25, 20, 32}
	before := 0
	calculateAscentBefore(points, &before)
	totalAscent := calculateAscent(points)
	if totalAscent != before {
		panic("mismatch")
	}
	calculateAscentBefore(points, &before)
	if before != 2*totalAscent || calculateAscent(points) != totalAscent {
		panic("mismatch")
	}
	fmt.Println(totalAscent)
}
```

```output
27
```
