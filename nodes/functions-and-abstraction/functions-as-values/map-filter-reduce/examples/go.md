```go run id=map-filter-reduce-go
package main

import (
	"fmt"
	"strconv"
	"strings"
)

func Map[T, U any](xs []T, f func(T) U) []U {
	out := make([]U, 0, len(xs))
	for _, x := range xs {
		out = append(out, f(x))
	}
	return out
}

func Filter[T any](xs []T, keep func(T) bool) []T {
	var out []T
	for _, x := range xs {
		if keep(x) {
			out = append(out, x)
		}
	}
	return out
}

func Reduce[T, A any](xs []T, init A, f func(A, T) A) A {
	acc := init
	for _, x := range xs {
		acc = f(acc, x)
	}
	return acc
}

func main() {
	nums := []int{1, 2, 3, 4, 5, 6}
	evens := Filter(nums, func(x int) bool { return x%2 == 0 })
	squares := Map(evens, func(x int) int { return x * x })
	fmt.Println(strings.Join(Map(squares, strconv.Itoa), " "))
	fmt.Println(Reduce(squares, 0, func(acc, x int) int { return acc + x }))
}
```

```output for=map-filter-reduce-go
4 16 36
56
```

- Verdict: idiom. The Go standard library has no map, filter or reduce for slices. A program writes them as generic functions with a `for` loop and a `range` clause.
