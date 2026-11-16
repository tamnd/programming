```go run id=pipeline
package main

import (
	"fmt"
	"iter"
)

func naturals() iter.Seq[int] {
	return func(yield func(int) bool) {
		for n := 1; ; n++ {
			if !yield(n) {
				return
			}
		}
	}
}

func mapSeq[T, U any](seq iter.Seq[T], f func(T) U) iter.Seq[U] {
	return func(yield func(U) bool) {
		for item := range seq {
			if !yield(f(item)) {
				return
			}
		}
	}
}

func filterSeq[T any](seq iter.Seq[T], test func(T) bool) iter.Seq[T] {
	return func(yield func(T) bool) {
		for item := range seq {
			if test(item) && !yield(item) {
				return
			}
		}
	}
}

func main() {
	squares := mapSeq(naturals(), func(n int) int {
		fmt.Println("square", n)
		return n * n
	})
	odd := filterSeq(squares, func(s int) bool { return s%2 == 1 })

	var first []int
	for s := range odd {
		first = append(first, s)
		if len(first) == 2 {
			break
		}
	}
	fmt.Println(first)
}
```

```output for=pipeline
square 1
square 2
square 3
[1 9]
```

- Verdict: stdlib. An `iter.Seq[T]` is a push iterator: a function that calls `yield` for each element. The `break` in the `for range` loop makes `yield` return false, and each stage then returns, so the infinite source stops.
- The standard library has `iter.Seq` sources, such as `maps.Keys` and `slices.Values`, but no `map` or `filter` stages, so programs write small functions like `mapSeq`.
