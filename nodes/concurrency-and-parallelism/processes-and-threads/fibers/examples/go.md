```go run id=goroutine-fringe
package main

import "fmt"

// walk sends each leaf on ch. The send waits until a receiver takes the value,
// so walk pauses there, deep in the recursion.
func walk(tree any, ch chan<- int) {
	switch t := tree.(type) {
	case []any:
		for _, child := range t {
			walk(child, ch)
		}
	case int:
		ch <- t
	}
}

func start(tree any) <-chan int {
	ch := make(chan int) // unbuffered
	go func() {
		walk(tree, ch)
		close(ch)
	}()
	return ch
}

func sameFringe(x, y any) bool {
	cx, cy := start(x), start(y)
	for {
		a, okA := <-cx
		b, okB := <-cy
		if okA != okB || a != b {
			return false
		}
		if !okA {
			return true
		}
	}
}

func main() {
	a := []any{[]any{1, 2}, []any{3, []any{4}}}
	b := []any{1, []any{2, []any{3, 4}}}
	c := []any{[]any{1, 2}, []any{3, 5}}

	for leaf := range start(a) {
		fmt.Println("got", leaf)
	}
	fmt.Println("same fringe a b:", sameFringe(a, b))
	fmt.Println("same fringe a c:", sameFringe(a, c))
}
```

```output for=goroutine-fringe
got 1
got 2
got 3
got 4
same fringe a b: true
same fringe a c: false
```

- Verdict: absent. Go has no fibers that the program switches by hand. A goroutine with an unbuffered channel gives the same stackful pause: `walk` can wait on a send at any depth.
- The Go runtime, not the program, decides when each goroutine runs. Each goroutine has its own stack, which the runtime grows when necessary.
- When `sameFringe` returns early, the unfinished goroutines stay blocked on their sends. In a long-running program, add a `done` channel so that they can stop.
