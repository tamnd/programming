```go run id=int32
package main

import (
	"errors"
	"fmt"
	"math"
)

func addInt32(a, b int32) (int32, error) {
	if (b > 0 && a > math.MaxInt32-b) || (b < 0 && a < math.MinInt32-b) {
		return 0, errors.New("int32 overflow")
	}
	return a + b, nil
}

func main() {
	var x int32 = math.MaxInt32
	fmt.Println("x + 1:", x+1)

	var u uint8 = 255
	fmt.Println("uint8 255 + 1:", u+1)

	_, err := addInt32(x, 1)
	fmt.Println("addInt32(x, 1):", err)
	sum, err := addInt32(x, -1)
	fmt.Println("addInt32(x, -1):", sum, err)
}
```

```output for=int32
x + 1: -2147483648
uint8 255 + 1: 0
addInt32(x, 1): int32 overflow
addInt32(x, -1): 2147483646 <nil>
```

- Verdict: idiom. Go wraps signed and unsigned values and never panics on overflow. The standard library has no checked add for signed integers, so a program tests the operands before the operation.
- The test compares with `math.MaxInt32-b`, which cannot overflow when `b` is positive.
