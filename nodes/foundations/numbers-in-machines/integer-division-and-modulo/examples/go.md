```go run id=truncated
package main

import (
	"fmt"
	"math/big"
)

func main() {
	a, n := -7, 3
	fmt.Println("-7 / 3:", a/n)
	fmt.Println("-7 % 3:", a%n)

	x, y := big.NewInt(-7), big.NewInt(3)
	fmt.Println("big Div(-7, 3):", new(big.Int).Div(x, y))
	fmt.Println("big Mod(-7, 3):", new(big.Int).Mod(x, y))
	fmt.Println("big Quo(-7, 3):", new(big.Int).Quo(x, y))
	fmt.Println("big Rem(-7, 3):", new(big.Int).Rem(x, y))
}
```

```output for=truncated
-7 / 3: -2
-7 % 3: -1
big Div(-7, 3): -3
big Mod(-7, 3): 2
big Quo(-7, 3): -2
big Rem(-7, 3): -1
```

- Verdict: native. `/` truncates toward zero, and `%` has the sign of the dividend. For `big.Int`, `Div` and `Mod` are Euclidean, and `Quo` and `Rem` are truncated.
- An integer division by a zero variable panics at run time, and a division by a zero constant does not compile.
