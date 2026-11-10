```go run id=hof
package main

import (
	"cmp"
	"fmt"
	"slices"
)

func Map[T, U any](items []T, f func(T) U) []U {
	result := make([]U, 0, len(items))
	for _, item := range items {
		result = append(result, f(item))
	}
	return result
}

func compose[A, B, C any](f func(B) C, g func(A) B) func(A) C {
	return func(x A) C { return f(g(x)) }
}

func main() {
	words := []string{"pear", "fig", "banana"}
	lengths := Map(words, func(w string) int { return len(w) })
	fmt.Println(lengths)
	sorted := slices.Clone(words)
	slices.SortFunc(sorted, func(a, b string) int { return cmp.Compare(len(a), len(b)) })
	fmt.Println(sorted)
	addThenDouble := compose(func(n int) int { return n * 2 }, func(n int) int { return n + 1 })
	fmt.Println(addThenDouble(5))
}
```

```output for=hof
[4 3 6]
[fig pear banana]
12
```

- Verdict: native. A function type such as `func(T) U` is a type like any other. Function literals, named functions and method values all have function types.
- The standard library has no `Map` or `Filter` for slices. A generic function such as `Map` above is the usual form. The package `slices` takes functions in `SortFunc`, `IndexFunc`, `ContainsFunc` and others.
- The zero value of a function type is `nil`. A call of a nil function panics (see Pitfalls).
- Function values are not comparable, except with `nil`.
