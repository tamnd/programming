```go run id=mathbig
package main

import (
	"fmt"
	"math/big"
)

func yesNo(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	two := big.NewInt(2)
	fmt.Println("2^64:", new(big.Int).Exp(two, big.NewInt(64), nil))
	fmt.Println("2^100:", new(big.Int).Exp(two, big.NewInt(100), nil))
	fmt.Println("30!:", new(big.Int).MulRange(1, 30))
	z := big.NewInt(1)
	for i := 0; i < 4; i++ {
		z.Mul(z, big.NewInt(1000000))
	}
	fmt.Println("z after 4 Mul calls:", z)
	x := new(big.Int).Exp(two, big.NewInt(64), nil)
	fmt.Println("2^64 BitLen:", x.BitLen())
	fmt.Println("2^64 IsInt64:", yesNo(x.IsInt64()))
	fmt.Println("2^64 IsUint64:", yesNo(x.IsUint64()))
	r := new(big.Rat).Add(big.NewRat(1, 3), big.NewRat(1, 6))
	fmt.Println("1/3 + 1/6:", r)
}
```

```output for=mathbig
2^64: 18446744073709551616
2^100: 1267650600228229401496703205376
30!: 265252859812191058636308480000000
z after 4 Mul calls: 1000000000000000000000000
2^64 BitLen: 65
2^64 IsInt64: no
2^64 IsUint64: no
1/3 + 1/6: 1/2
```

- Verdict: stdlib. `math/big` has `Int`, `Rat` and `Float`, and each operation is a method that writes its result into the receiver.
- `z.Mul(z, ...)` uses `z` as an operand and as the result, so the loop reuses the memory of `z`.
- `IsInt64` and `IsUint64` tell whether a value fits in a fixed-width type before a conversion.
