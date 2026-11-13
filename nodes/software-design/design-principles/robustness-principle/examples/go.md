```go run id=port-setting
package main

import (
	"fmt"
	"strconv"
)

// lenient reads digits until it cannot, like parseInt in JavaScript.
func lenient(s string) int {
	n := 0
	for _, r := range s {
		if r < '0' || r > '9' {
			break
		}
		n = n*10 + int(r-'0')
	}
	return n
}

func main() {
	text := "8080x"
	fmt.Printf("lenient: %d\n", lenient(text))
	if n, err := strconv.Atoi(text); err != nil {
		fmt.Println("strict: rejected")
	} else {
		fmt.Printf("strict: %d\n", n)
	}
}
```

```output
lenient: 8080
strict: rejected
```
