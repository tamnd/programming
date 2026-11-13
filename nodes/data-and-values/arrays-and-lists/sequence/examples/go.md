```go run id=iter-seq
package main

import (
	"fmt"
	"iter"
	"maps"
	"slices"
)

// Fib returns a lazy sequence. It computes each number when the loop asks for it.
func Fib() iter.Seq[int] {
	return func(yield func(int) bool) {
		a, b := 0, 1
		for {
			if !yield(a) {
				return // the loop stopped early
			}
			a, b = b, a+b
		}
	}
}

func main() {
	// range over a function calls it with a yield function. break makes yield return false.
	for n := range Fib() {
		if n > 50 {
			break
		}
		fmt.Print(n, " ")
	}
	fmt.Println()

	// slices.Values turns a slice into a sequence, and slices.Collect turns a finite sequence into a slice.
	words := []string{"b", "a", "c"}
	fmt.Println(slices.Collect(slices.Values(words)), slices.Sorted(slices.Values(words)))

	// Seq2 gives two values per step, such as an index and an item.
	for i, w := range slices.Backward(words) {
		fmt.Print(i, w, " ")
	}
	fmt.Println()

	// A map gives a sequence of keys in no fixed order, so the program sorts them.
	m := map[string]int{"y": 2, "x": 1, "z": 3}
	fmt.Println(slices.Sorted(maps.Keys(m)))
}
```

```output
0 1 1 2 3 5 8 13 21 34
[b a c] [a b c]
2c 1a 0b
[x y z]
```

- Verdict: stdlib. Since Go 1.23, an iter.Seq is a function that a range loop calls, and break stops an infinite sequence. slices.Values, slices.Collect, slices.Sorted and slices.Backward change between slices and sequences. maps.Keys gives the keys of a map as a sequence, in no fixed order, so the program sorts them.
