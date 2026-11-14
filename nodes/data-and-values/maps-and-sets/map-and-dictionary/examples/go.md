```go run id=map-type
package main

import (
	"fmt"
	"maps"
	"slices"
)

func main() {
	// A map literal, then insert, update, read and delete.
	m := map[string]int{"x": 1, "y": 2}
	m["z"] = 3
	m["x"] += 10
	delete(m, "y")
	fmt.Println(m["x"], len(m))

	// A missing key gives the zero value. The two-value form tells the cases apart.
	v, ok := m["w"]
	fmt.Println(m["w"], v, ok)
	m["zero"] = 0
	v, ok = m["zero"]
	fmt.Println(v, ok)

	// The order of a range loop over a map is not specified, so sort the keys to print.
	fmt.Println(slices.Sorted(maps.Keys(m)))

	// A nil map reads like an empty map, but a write panics.
	var nilMap map[string]int
	fmt.Println(nilMap["a"], len(nilMap))
	func() {
		defer func() { fmt.Println("panic:", recover()) }()
		nilMap["a"] = 1
	}()

	// A struct of comparable fields can be a key. A slice cannot.
	type point struct{ x, y int }
	grid := map[point]string{{0, 0}: "start", {2, 3}: "end"}
	fmt.Println(grid[point{2, 3}])
}
```

```output
11 2
0 0 false
0 true
[x z zero]
0 0
panic: assignment to entry in nil map
end
```

- Verdict: native. A missing key gives the zero value, and the two-value form v, ok tells a missing key from a stored zero. The order of a range loop is not specified, so the example sorts the keys. A nil map reads as empty and panics on a write. A struct of comparable fields can be a key.
