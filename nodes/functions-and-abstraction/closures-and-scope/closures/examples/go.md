```go run id=counter
package main

import "fmt"

func makeCounter() func() int {
	count := 0
	return func() int {
		count++
		return count
	}
}

func main() {
	first := makeCounter()
	second := makeCounter()
	fmt.Println(first(), first(), first())
	fmt.Println(second())
}
```

```output for=counter
1 2 3
1
```

- Verdict: native. A function literal is a closure. It shares the variables of the surrounding function with that function.
- The compiler puts a captured variable on the heap when the closure can outlive the call. For this program, `go build -gcflags=-m` reports `moved to heap: count` and `func literal escapes to heap`.
- Each iteration of a `for` loop has its own loop variable, so closures made in a loop see different values.
