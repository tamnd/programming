```go run id=match-guards-go
package main

import (
	"fmt"
	"strings"
)

func describe(n int) string {
	switch {
	case n == 0:
		return "zero"
	case n < 0:
		return "negative"
	case n < 10 && n%2 == 0:
		return "small even"
	case n < 10:
		return "small odd"
	default:
		return "large"
	}
}

func main() {
	var words []string
	for _, n := range []int{-5, 0, 4, 7, 12} {
		words = append(words, describe(n))
	}
	fmt.Println(strings.Join(words, ", "))
}
```

```output for=match-guards-go
negative, zero, small even, small odd, large
```

- Verdict: idiom. Go has no patterns, so it has no guards. A switch with no expression tests each case as a boolean condition, in order, and each condition does the work of a guard.
