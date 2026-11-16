```go run id=terminating-panic
package main

import (
	"fmt"
	"strconv"
)

// A call to panic is a terminating statement, so no return must follow it.
func mustPort(s string) int {
	n, err := strconv.Atoi(s)
	if err == nil {
		return n
	}
	panic("bad port: " + s)
}

// A for loop without a condition and without break is a terminating statement too.
func firstNegative(ch <-chan int) int {
	for {
		if n := <-ch; n < 0 {
			return n
		}
	}
}

func main() {
	fmt.Println(mustPort("8080"))
	func() {
		defer func() {
			fmt.Println("recovered:", recover())
		}()
		mustPort("x")
	}()

	ch := make(chan int, 3)
	ch <- 1
	ch <- 2
	ch <- -1
	fmt.Println(firstNegative(ch))
}
```

```output for=terminating-panic
8080
recovered: bad port: x
-1
```

- Verdict: idiom. Go has no bottom type. The specification makes a call to `panic` and an endless `for` loop terminating statements, so `mustPort` and `firstNegative` need no `return` at the end. A call to your own function that always panics does not count, so a function that ends with it still needs a `return`.
