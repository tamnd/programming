```go run id=scaled
package main

import "fmt"

const shift = 16

func mul(a, b int32) int64 {
	return (int64(a)*int64(b) + 1<<(shift-1)) >> shift
}

func main() {
	a := int32(3 << (shift - 1))
	b := int32(9 << (shift - 2))
	fmt.Println("1.5 * 2.25:", float64(mul(a, b))/65536)
	big := int32(300 << shift)
	p := mul(big, big)
	fmt.Println("300 * 300 in int64:", float64(p)/65536)
	fmt.Println("300 * 300 in int32:", float64(int32(p))/65536)
	cents := int64(1999 * 3)
	fmt.Printf("3 x 19.99: %d.%02d\n", cents/100, cents%100)
	fmt.Println("-7 >> 1:", int32(-7)>>1, "and -7 / 2:", int32(-7)/2)
}
```

```output for=scaled
1.5 * 2.25: 3.375
300 * 300 in int64: 90000
300 * 300 in int32: 24464
3 x 19.99: 59.97
-7 >> 1: -4 and -7 / 2: -3
```

- Verdict: idiom. Go has no fixed-point type. The block multiplies two Q16.16 `int32` values in an `int64`.
- The conversion of the product to `int32` keeps the low 32 bits, so 90000 wraps to 24464 with no error.
- A right shift rounds toward minus infinity, and `/` truncates toward zero, so the two rescale methods differ for negative values.
