```go run id=comparable-map-keys
package main

import (
	"fmt"
	"math"
)

type Point struct{ X, Y float64 }

func main() {
	m := map[Point]int{}
	m[Point{1, 2}]++
	m[Point{1, 2}]++
	fmt.Println(len(m), m[Point{1, 2}])

	nan := math.NaN()
	m[Point{nan, 0}]++
	m[Point{nan, 0}]++
	fmt.Println(len(m), m[Point{nan, 0}])

	var keys = map[any]bool{}
	defer func() { fmt.Println("panic:", recover()) }()
	keys[[]int{1}] = true
}
```

```output for=comparable-map-keys
1 2
3 0
panic: runtime error: hash of unhashable type []int
```

- Verdict: native. Go has no user hash: a map key must be comparable, and the map uses the built-in `==` and the built-in hash of the key type, so they always agree.
- A float field breaks reflexivity: `NaN != NaN`, so each `Point{NaN, 0}` is a new key that a lookup cannot find.
- A key of interface type is checked at run time. A slice is not comparable, so storing one as a key of `map[any]bool` panics.
