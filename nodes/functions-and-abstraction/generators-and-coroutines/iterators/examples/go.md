```go run id=countdown
package main

import (
	"fmt"
	"iter"
)

func Countdown(start int) iter.Seq[int] {
	return func(yield func(int) bool) {
		for i := start; i > 0; i-- {
			if !yield(i) {
				fmt.Println("stopped")
				return
			}
		}
	}
}

func main() {
	for n := range Countdown(3) {
		fmt.Println(n)
	}
	for n := range Countdown(5) {
		if n == 4 {
			break
		}
		fmt.Println(n)
	}
	next, stop := iter.Pull(Countdown(2))
	defer stop()
	for {
		n, ok := next()
		if !ok {
			break
		}
		fmt.Println("pulled", n)
	}
}
```

```output for=countdown
3
2
1
5
stopped
pulled 2
pulled 1
```

- Verdict: native. An iterator is a function of type `iter.Seq[V]`, `func(yield func(V) bool)`. A `range` loop over it gives the loop body to the iterator as `yield`.
- This is a push iterator: the iterator calls the loop. When the loop body runs `break`, `yield` returns `false`, and the iterator must return.
- `iter.Pull` turns a push iterator into a pull iterator, a `next` function and a `stop` function. Call `stop` when you finish early.
- `range` also works over slices, maps, strings, channels and integers. The order of a map is not specified (see Pitfalls on the concept page).
