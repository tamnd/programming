```go run id=float
package main

import (
	"fmt"
	"math"
)

func main() {
	x := 0.1 + 0.2
	fmt.Println("0.1 + 0.2 =", x)

	a, b := 0.1, 0.2
	y := a + b
	fmt.Println("a + b     =", y)
	fmt.Println("a + b == 0.3:", y == 0.3)

	fmt.Printf("0.1 to 20 digits: %.20f\n", 0.1)
	fmt.Println("next after 1.0:", math.Nextafter(1.0, 2.0))

	var f32 float32 = 16777217
	fmt.Println("float32(16777217) =", f32)

	nan := math.NaN()
	m := map[float64]int{}
	m[nan] = 1
	m[nan] = 2
	fmt.Println("map with two NaN keys has length", len(m))
	_, ok := m[nan]
	fmt.Println("lookup of NaN finds it:", ok)
	clear(m)
	fmt.Println("length after clear:", len(m))
}
```

```output
0.1 + 0.2 = 0.3
a + b     = 0.30000000000000004
a + b == 0.3: false
0.1 to 20 digits: 0.10000000000000000555
next after 1.0: 1.0000000000000002
float32(16777217) = 1.6777216e+07
map with two NaN keys has length 2
lookup of NaN finds it: false
length after clear: 0
```

- Verdict: native. `float64` is IEEE 754 binary64 and `float32` is binary32.
- `0.1 + 0.2` is a constant expression. Go evaluates constant expressions exactly and rounds only when it stores the result, so `x` is the float nearest to 0.3. The sum of the variables `a` and `b` is rounded after each step, as at run time.
- A `float32` has a 24-bit significand, so 16777217 (2^24 + 1) rounds to 16777216.
- `NaN` is not equal to itself, so each `NaN` key in a map is a new entry, and a lookup cannot find it. The built-in `clear` removes such entries.
