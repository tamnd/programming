```go run id=continuations
package main

import (
	"errors"
	"fmt"
	"strconv"
)

// factK passes n! to k instead of returning it.
func factK(n int, k func(int) int) int {
	if n == 0 {
		return k(1)
	}
	return factK(n-1, func(r int) int { return k(n * r) })
}

// parseK has one continuation for success and one for failure.
func parseK(s string, ok func(int), fail func(error)) {
	n, err := strconv.Atoi(s)
	if err != nil {
		fail(errors.New("not a number: " + s))
		return
	}
	ok(n)
}

// sumK adds 1..n; each step wraps the continuation in a new closure.
func sumK(n int, k func(int) int) int {
	if n == 0 {
		return k(0)
	}
	return sumK(n-1, func(r int) int { return k(n + r) })
}

func main() {
	fmt.Println(factK(5, func(r int) int { return r }))
	fmt.Println(factK(5, func(r int) int { return r * 2 }))
	for _, s := range []string{"42", "4x"} {
		parseK(s,
			func(n int) { fmt.Println("ok", n+1) },
			func(err error) { fmt.Println("error:", err) })
	}
	fmt.Println(sumK(1_000_000, func(r int) int { return r })) // the stack grows, so deep CPS works
}
```

```output for=continuations
120
240
ok 43
error: not a number: 4x
500000500000
```

- Verdict: idiom. Go functions are values and close over their variables, so a continuation is a parameter of type `func(int) int`. `parseK` takes one continuation for success and one for failure, a style that Go code mostly writes as an `(int, error)` return value instead.
- Go has no tail call elimination either, but goroutine stacks grow on demand, so `sumK` can nest a million calls where Python and Java run out of stack. Each step allocates a closure on the heap, so the CPS version is much slower than a loop.
