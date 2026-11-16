```go run id=strategy-go
package main

import (
	"errors"
	"fmt"
)

func divide(a, b int) (int, error) {
	if b == 0 {
		return 0, errors.New("division by zero")
	}
	return a / b, nil
}

func main() {
	for _, p := range [][2]int{{10, 2}, {10, 0}} {
		q, err := divide(p[0], p[1])
		if err != nil {
			fmt.Printf("%d / %d: error: %v\n", p[0], p[1], err)
			continue
		}
		fmt.Printf("%d / %d = %d\n", p[0], p[1], q)
	}
}
```

```output for=strategy-go
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: native. Strategy: error values. A function returns an `error` as its last result, and the caller tests it with `if err != nil`. A panic is for a bug or a state that the program cannot recover from, not for an expected error.
