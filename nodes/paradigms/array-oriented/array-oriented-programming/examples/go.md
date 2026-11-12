```go run id=generic-zip
package main

import "fmt"

type Number interface{ ~int | ~float64 }

// ZipWith applies f to each pair of elements. The lengths must match.
func ZipWith[T Number](f func(T, T) T, a, b []T) []T {
	if len(a) != len(b) {
		panic(fmt.Sprintf("length error: %d and %d", len(a), len(b)))
	}
	out := make([]T, len(a))
	for i := range a {
		out[i] = f(a[i], b[i])
	}
	return out
}

func Reduce[T Number](f func(T, T) T, start T, a []T) T {
	acc := start
	for _, x := range a {
		acc = f(acc, x)
	}
	return acc
}

func main() {
	a := []int{3, 1, 4, 1, 5}
	b := []int{2, 7, 1, 8, 2}
	add := func(x, y int) int { return x + y }
	mul := func(x, y int) int { return x * y }

	fmt.Println(ZipWith(add, a, b))
	fmt.Println(Reduce(add, 0, ZipWith(mul, a, b)))
}
```

```output for=generic-zip
[5 8 5 9 7]
35
```

- Verdict: absent. Go has no operations on whole arrays or slices, and no `map` or `reduce` in the standard library. Loops are the normal style, and generic functions such as `ZipWith` give the array vocabulary once. The gc compiler does little automatic vectorization of loops, so libraries such as Gonum use assembly for their fast paths.
