```go run id=return-value-go
package main

import "fmt"

func absolute(n int) int {
	if n < 0 {
		return -n
	}
	return n
}

func main() {
	fmt.Println(absolute(-7))
	fmt.Println(absolute(5) + 1)
}
```

```output for=return-value-go
7
6
```

- Verdict: native. The result type follows the parameter list, and the compiler checks that the body ends in a `return`. A Go function can also return several values.
