```go run id=rat
package main

import (
	"fmt"
	"math/big"
)

func main() {
	fmt.Println("NewRat(6, -8):", big.NewRat(6, -8))
	s := new(big.Rat).Add(big.NewRat(1, 10), big.NewRat(2, 10))
	fmt.Println("1/10 + 2/10:", s)
	fmt.Println("RatString:", s.RatString())
	fmt.Println("SetFloat64(0.1):", new(big.Rat).SetFloat64(0.1))
	r, _ := new(big.Rat).SetString("0.1")
	fmt.Println("SetString(\"0.1\"):", r)
	fmt.Println("1/3 FloatString(5):", big.NewRat(1, 3).FloatString(5))
	fmt.Println("2/3 FloatString(5):", big.NewRat(2, 3).FloatString(5))
	defer func() {
		fmt.Println("Quo by 0:", recover())
	}()
	new(big.Rat).Quo(big.NewRat(1, 1), new(big.Rat))
}
```

```output for=rat
NewRat(6, -8): -3/4
1/10 + 2/10: 3/10
RatString: 3/10
SetFloat64(0.1): 3602879701896397/36028797018963968
SetString("0.1"): 1/10
1/3 FloatString(5): 0.33333
2/3 FloatString(5): 0.66667
Quo by 0: division by zero
```

- Verdict: stdlib. `math/big` has `Rat`, with `big.Int` parts that do not overflow.
- `SetFloat64` gives the exact value of the float, and `SetString` reads a decimal string exactly.
- `FloatString` rounds the last digit to nearest.
