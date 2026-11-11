```go run id=closures
package main

import (
	"fmt"
	"strings"
)

func add(a int) func(int) int {
	return func(b int) int { return a + b }
}

func main() {
	addTen := add(10)
	fmt.Println(addTen(5), add(1)(2))

	var sb strings.Builder
	write := sb.WriteString
	write("hello, ")
	write("go")
	fmt.Println(sb.String())
}
```

```output for=closures
15 3
hello, go
```

- Verdict: idiom. A function can return a closure, so a curried function is easy to write. Go has no generic `partial` in the standard library.
- A method value such as `sb.WriteString` binds the receiver. It is a partial application of the method to its receiver.
