```go run id=traps
package main

import (
	"fmt"
	"math"
	"slices"
	"sort"
)

func main() {
	nan := math.NaN()
	fmt.Println("nan == nan:", nan == nan, "IsNaN:", math.IsNaN(nan))
	zero := 0.0
	fmt.Println("1/0:", 1/zero, "-1/0:", -1/zero, "0/0:", zero/zero)
	negZero := math.Copysign(0, -1)
	fmt.Println("-0 == 0:", negZero == 0, "Signbit:", math.Signbit(negZero))

	m := map[float64]string{}
	m[nan] = "a"
	m[nan] = "b"
	_, found := m[nan]
	fmt.Println("map size:", len(m), "found:", found)
	delete(m, nan)
	fmt.Println("after delete:", len(m))
	clear(m)
	fmt.Println("after clear:", len(m))

	values := []float64{3, nan, 1, 2}
	sort.Float64s(values)
	fmt.Println("sort.Float64s:", values)
	fmt.Println("min(1, NaN):", min(1.0, nan), "slices.Max:", slices.Max([]float64{1, nan, 2}))
}
```

```output for=traps
nan == nan: false IsNaN: true
1/0: +Inf -1/0: -Inf 0/0: NaN
-0 == 0: true Signbit: true
map size: 2 found: false
after delete: 2
after clear: 0
sort.Float64s: [NaN 1 2 3]
min(1, NaN): NaN slices.Max: NaN
```

- Verdict: native. Go `float64` follows IEEE 754.
- Each NaN key is a new entry in a map, and no lookup finds it. `delete` with a NaN key removes nothing, but `clear` removes all entries.
- `sort.Float64s` puts NaN values first. The built-in `min` and `slices.Max` return NaN when a NaN is present.
