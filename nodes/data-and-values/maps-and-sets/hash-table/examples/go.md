```go run id=map-hashing
package main

import (
	"fmt"
	"math"
)

type point struct{ x, y int }

func main() {
	// A struct of comparable fields is a key. Go hashes and compares all its fields.
	grid := map[point]string{{1, 2}: "found"}
	fmt.Println(grid[point{1, 2}])

	// NaN is not equal to itself, so each NaN insert adds a new entry
	// and no lookup finds it again.
	m := map[float64]int{}
	m[math.NaN()] = 1
	m[math.NaN()] = 2
	v, ok := m[math.NaN()]
	fmt.Println(len(m), v, ok)

	// clear removes all entries, including the NaN keys that delete cannot reach.
	clear(m)
	fmt.Println(len(m))

	// 0.0 and -0.0 are equal, so they are one key.
	m[0.0] = 1
	m[math.Copysign(0, -1)] = 2
	fmt.Println(len(m), m[0])

	// A size hint makes room for 1000 entries before the first insert.
	big := make(map[int]int, 1000)
	for i := range 1000 {
		big[i] = i * i
	}
	fmt.Println(len(big), big[999])

	// An interface key that holds a slice panics at run time, because a slice cannot be hashed.
	keys := map[any]int{}
	defer func() { fmt.Println("panic:", recover()) }()
	keys[[]int{1}] = 1
}
```

```output
found
2 0 false
0
1 2
1000 998001
panic: runtime error: hash of unhashable type []int
```

- Verdict: native. Go hashes and compares keys of any comparable type, such as a struct. NaN keys are never found again, and only clear removes them. 0.0 and -0.0 are one key. make with a size hint makes room before the inserts. An interface key that holds a slice panics, because a slice cannot be hashed. Since Go 1.24 the map is a Swiss table.
