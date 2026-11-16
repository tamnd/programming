```go run id=infinite
package main

import (
	"fmt"
	"iter"
)

func naturalsFrom(n int) iter.Seq[int] {
	return func(yield func(int) bool) {
		for ; ; n++ {
			if !yield(n) {
				return
			}
		}
	}
}

func iterate(f func(int) int, x int) iter.Seq[int] {
	return func(yield func(int) bool) {
		for ; ; x = f(x) {
			if !yield(x) {
				return
			}
		}
	}
}

func filter(seq iter.Seq[int], test func(int) bool) iter.Seq[int] {
	return func(yield func(int) bool) {
		for n := range seq {
			if test(n) && !yield(n) {
				return
			}
		}
	}
}

func take(seq iter.Seq[int], count int) []int {
	var result []int
	if count <= 0 {
		return result
	}
	for n := range seq {
		result = append(result, n)
		if len(result) == count {
			break
		}
	}
	return result
}

func isPrime(n int) bool {
	for d := 2; d*d <= n; d++ {
		if n%d == 0 {
			return false
		}
	}
	return true
}

func main() {
	fmt.Println(take(filter(naturalsFrom(2), isPrime), 10))
	fmt.Println(take(iterate(func(x int) int { return 2 * x }, 1), 8))
}
```

```output for=infinite
[2 3 5 7 11 13 17 19 23 29]
[1 2 4 8 16 32 64 128]
```

- Verdict: stdlib. An `iter.Seq` function with an endless loop is an infinite sequence. It must check the result of `yield` and return when it is false: after a `break` in the consumer, `yield` returns false, and the `iter` documentation states that `yield` panics if it is called again.
- The standard library has no `take` or `filter` for `iter.Seq`, so the program writes them.
