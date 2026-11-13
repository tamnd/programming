```go run id=range-over-int
package main

import (
	"fmt"
	"iter"
	"slices"
)

// Range gives start, start+step, ... and stops before stop.
func Range(start, stop, step int) iter.Seq[int] {
	return func(yield func(int) bool) {
		for i := start; (step > 0 && i < stop) || (step < 0 && i > stop); i += step {
			if !yield(i) {
				return
			}
		}
	}
}

func main() {
	// Since Go 1.22, range over an integer n gives 0 through n-1.
	for i := range 5 {
		fmt.Print(i, " ")
	}
	fmt.Println()

	// If n <= 0, the loop runs zero times.
	n := -3
	count := 0
	for range n {
		count++
	}
	fmt.Println("iterations:", count)

	// An iterator function gives a range with any start and step.
	fmt.Println(slices.Collect(Range(2, 7, 1)), slices.Collect(Range(10, 0, -3)))

	// A slice expression a[low:high] uses a half-open range of indices.
	a := []string{"a", "b", "c", "d", "e"}
	fmt.Println(a[1:3], len(a[1:3]), a[:2], a[3:])
}
```

```output
0 1 2 3 4
iterations: 0
[2 3 4 5 6] [10 7 4 1]
[b c] 2 [a b] [d e]
```

- Verdict: native. Since Go 1.22, for i := range 5 gives 0 to 4, and a count of 0 or less runs no iterations. An iter.Seq function gives a range with any start and step. A slice expression a[low:high] uses a half-open range of indices.
