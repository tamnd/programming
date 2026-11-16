```go run id=money
package main

import (
	"fmt"
	"math/big"
)

func main() {
	total := 0.0
	for i := 0; i < 10; i++ {
		total += 0.1
	}
	fmt.Println("loop:", total)
	fmt.Printf("0.1 exact: %.55f\n", 0.1)

	a, b := 0.1, 0.2
	fmt.Println("variables 0.1 + 0.2:", a+b)
	fmt.Println("constants 0.1 + 0.2:", 0.1+0.2)

	sum := new(big.Rat)
	tenth := big.NewRat(1, 10)
	for i := 0; i < 10; i++ {
		sum.Add(sum, tenth)
	}
	fmt.Println("big.Rat sum:", sum.RatString())
}
```

```output for=money
loop: 0.9999999999999999
0.1 exact: 0.1000000000000000055511151231257827021181583404541015625
variables 0.1 + 0.2: 0.30000000000000004
constants 0.1 + 0.2: 0.3
big.Rat sum: 1
```

- Verdict: native. Go `float64` is binary64.
- Go evaluates constant expressions exactly, so the constant `0.1+0.2` has no rounding error until it becomes a `float64`. The same sum of two variables rounds.
- `big.Rat` keeps one tenth as an exact fraction.
