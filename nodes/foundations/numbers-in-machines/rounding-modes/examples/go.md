```go run id=ties
package main

import (
	"fmt"
	"math"
)

func main() {
	fmt.Println("    x Round  Even Floor  Ceil Trunc")
	for _, x := range []float64{0.5, 1.5, 2.5, -0.5, -1.5, -2.5} {
		fmt.Printf("%5.1f", x)
		for _, r := range []float64{math.Round(x), math.RoundToEven(x), math.Floor(x), math.Ceil(x), math.Trunc(x)} {
			fmt.Printf("%6.0f", r+0)
		}
		fmt.Println()
	}

	fmt.Printf("Sprintf %%.0f: %.0f %.0f %.0f\n", 0.5, 1.5, 2.5)
	fmt.Printf("Sprintf %%.2f of 2.675: %.2f\n", 2.675)
	x := -2.7
	fmt.Println("int(-2.7):", int(x))
	fmt.Println("-7 / 2:", -7/2)
}
```

```output for=ties
    x Round  Even Floor  Ceil Trunc
  0.5     1     0     0     1     0
  1.5     2     2     1     2     1
  2.5     3     2     2     3     2
 -0.5    -1     0    -1     0     0
 -1.5    -2    -2    -2    -1    -1
 -2.5    -3    -2    -3    -2    -2
Sprintf %.0f: 0 2 2
Sprintf %.2f of 2.675: 2.67
int(-2.7): -2
-7 / 2: -3
```

- Verdict: native. `math.Round` rounds half away from zero, and `math.RoundToEven` rounds ties to even.
- Several results are negative zero. The table adds 0 to each result, so they print as 0.
- `fmt` rounds ties to even when it prints, so `%.0f` gives 0, 2 and 2.
- A conversion to `int` and integer division both truncate toward zero.
