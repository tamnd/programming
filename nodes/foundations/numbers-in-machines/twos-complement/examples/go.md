```go run id=negation
package main

import (
	"fmt"
	"math"
)

func yesNo(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	x := int32(6)
	fmt.Println("-x == ^x + 1:", yesNo(-x == ^x+1))
	v := int8(-6)
	fmt.Printf("bits of -6 as int8: %08b\n", uint8(v))
	fmt.Printf("int8(-6) widened to int16: %016b\n", uint16(int16(v)))

	m := int32(math.MinInt32)
	fmt.Println("-m:", -m)
	fmt.Println("m / -1:", m/-1)
}
```

```output for=negation
-x == ^x + 1: yes
bits of -6 as int8: 11111010
int8(-6) widened to int16: 1111111111111010
-m: -2147483648
m / -1: -2147483648
```

- Verdict: native. The Go specification says that each n-bit integer is represented with two's complement arithmetic.
- Go writes bitwise NOT as the unary operator `^`.
- Signed overflow wraps in Go, so the negation of the most negative `int32` is the same value, and the division by -1 gives the same value too. There is no panic.
