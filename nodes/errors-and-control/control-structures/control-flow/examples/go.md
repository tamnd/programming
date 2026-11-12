```go run id=defer-panic-and-goto
package main

import (
	"errors"
	"fmt"
)

// Deferred calls run when the function returns, last in first out.
func deferred() {
	for i := 1; i <= 3; i++ {
		defer fmt.Println("deferred", i)
	}
	fmt.Println("body done")
}

// recover in a deferred function stops a panic and lets the function return.
func safeDiv(a, b int) (q int, err error) {
	defer func() {
		if r := recover(); r != nil {
			err = errors.New(fmt.Sprint(r))
		}
	}()
	return a / b, nil
}

func main() {
	deferred()
	fmt.Println(safeDiv(7, 2))
	fmt.Println(safeDiv(1, 0))

	// A switch case does not fall through unless it says fallthrough.
	for _, n := range []int{1, 2, 3} {
		switch n {
		case 1:
			fmt.Println("one")
			fallthrough
		case 2:
			fmt.Println("one or two")
		default:
			fmt.Println("other")
		}
	}

	// goto jumps to a label in the same function.
	i := 0
loop:
	if i < 3 {
		i++
		goto loop
	}
	fmt.Println("i =", i)
}
```

```output for=defer-panic-and-goto
body done
deferred 3
deferred 2
deferred 1
3 <nil>
0 runtime error: integer divide by zero
one
one or two
one or two
other
i = 3
```

- Verdict: native. Deferred calls run when the function returns, in last-in, first-out order. A division by zero panics, and `recover` in a deferred function turns the panic into an error. A `switch` case does not fall through unless it says `fallthrough`. `goto` jumps back to a label to make a loop.
