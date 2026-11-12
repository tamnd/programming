```go run id=conversions-and-constants
package main

import (
	"fmt"
	"math"
	"math/big"
	"math/cmplx"
)

func main() {
	// Integer division truncates, and % takes the sign of the dividend.
	fmt.Println(7/2, -7/2, -7%2, 7.0/2)

	// Numeric types do not mix: int32 + int does not compile, so convert.
	var a int32 = 5
	var b int = 7
	fmt.Println(int(a)+b, float64(b)/2)

	// Untyped constants are exact, and they get a type only where they are used.
	const huge = 1 << 100
	fmt.Println(huge>>98, math.MaxInt64, math.MaxUint32)

	// A complex type is built in.
	fmt.Println(cmplx.Sqrt(-1), real(3+4i), cmplx.Abs(3+4i))

	// Floating-point division by zero gives infinity at run time.
	zero := 0.0
	fmt.Println(1/zero, math.IsNaN(zero/zero))

	// math/big gives exact integers and rationals.
	f := new(big.Int).MulRange(1, 30)
	r := new(big.Rat).Add(big.NewRat(1, 10), big.NewRat(2, 10))
	fmt.Println(f, r, 0.1+0.2 == 0.3)
}
```

```output for=conversions-and-constants
3 -3 -1 3.5
12 3.5
4 9223372036854775807 4294967295
(0+1i) 3 5
+Inf true
265252859812191058636308480000000 3/10 true
```

- Verdict: native. `/` truncates for integers, and `7.0/2` is `3.5`. `int32` and `int` are different types, so the program converts before it adds them. Untyped constants are exact, so `1 << 100` is a valid constant, and `0.1+0.2 == 0.3` is true because all three values are constants. `complex128` is built in. `math/big` gives exact integers and rationals.
