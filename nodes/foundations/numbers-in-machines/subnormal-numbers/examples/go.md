```go run id=tiny
package main

import (
	"fmt"
	"math"
)

const minNormal = 0x1p-1022

func isSubnormal(x float64) bool {
	return x != 0 && math.Abs(x) < minNormal
}

func yes(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	var smallest float64 = math.SmallestNonzeroFloat64
	fmt.Println("SmallestNonzeroFloat64:", smallest)
	fmt.Println("smallest normal:", minNormal)
	fmt.Printf("bits: %#016x %#016x\n", math.Float64bits(smallest), math.Float64bits(minNormal))

	x := 1.25 * minNormal
	y := minNormal
	fmt.Println("x - y:", x-y, "subnormal:", yes(isSubnormal(x-y)))

	frac, exp := math.Frexp(smallest)
	fmt.Println("Frexp(smallest):", frac, exp)
	fmt.Println("smallest / 2:", smallest/2)
}
```

```output for=tiny
SmallestNonzeroFloat64: 5e-324
smallest normal: 2.2250738585072014e-308
bits: 0x0000000000000001 0x0010000000000000
x - y: 5.562684646268003e-309 subnormal: yes
Frexp(smallest): 0.5 -1073
smallest / 2: 0
```

- Verdict: native. Go `float64` follows IEEE 754 and has subnormals.
- `math.SmallestNonzeroFloat64` is the smallest subnormal. Go has no constant for the smallest normal value and no `IsSubnormal`, so the code writes the constant as the hex literal `0x1p-1022`.
- `math.Frexp` normalizes a subnormal: it gives a fraction in `[0.5, 1)` and an exponent below -1022.
