```go run id=generic
package main

import (
	"fmt"
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

func Reduce[T, A any](xs []T, acc A, f func(A, T) A) A {
	for _, x := range xs {
		acc = f(acc, x)
	}
	return acc
}

func main() {
	words := []string{"map", "filter", "reduce", "fold", "zip"}
	long := Filter(words, func(w string) bool { return len(w) > 3 })
	fmt.Println(Map(long, strings.ToUpper))
	fmt.Println(Reduce(words, 0, func(n int, w string) int { return n + len(w) }))
}
```

```output for=generic
[FILTER REDUCE FOLD]
22
```

- Verdict: idiom. Go has functions as values, closures and generics, but the standard library has no `Map`, `Filter` or `Reduce` for slices, and the usual style is a loop. Slices and maps are mutable, and there is no way to mark a value as immutable.
