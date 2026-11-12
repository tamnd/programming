```go run id=comparable-keys-and-maphash
package main

import (
	"fmt"
	"hash/maphash"
	"math"
)

type Point struct{ X, Y int } // comparable: a struct of comparable fields

func main() {
	points := map[Point]string{{1, 2}: "a"}
	fmt.Println("struct key:", points[Point{1, 2}])

	// A slice cannot be a key; an array of fixed length can.
	paths := map[[2]string]bool{{"home", "ada"}: true}
	fmt.Println("array key:", paths[[2]string{"home", "ada"}])

	// NaN is not equal to itself, so each NaN key is a new entry
	m := map[float64]int{}
	m[math.NaN()] = 1
	m[math.NaN()] = 2
	_, found := m[math.NaN()]
	fmt.Println("NaN keys:", len(m), "found:", found)

	// maphash: the same seed gives the same hash; seeds are random per call
	s1, s2 := maphash.MakeSeed(), maphash.MakeSeed()
	fmt.Println("same seed:", maphash.String(s1, "hello") == maphash.String(s1, "hello"))
	fmt.Println("two seeds:", maphash.String(s1, "hello") == maphash.String(s2, "hello"))
	fmt.Println("comparable:", maphash.Comparable(s1, Point{1, 2}) == maphash.Comparable(s1, Point{1, 2}))
}
```

```output for=comparable-keys-and-maphash
struct key: a
array key: true
NaN keys: 2 found: false
same seed: true
two seeds: false
comparable: true
```

```go fails expect=compile id=slice-key
package main

import "fmt"

func main() {
	seen := map[[]string]bool{}
	seen[[]string{"home", "ada"}] = true
	fmt.Println(len(seen))
}
```

- Verdict: native. Maps are built in, and the runtime hashes any comparable key with a random seed for each map: numbers, strings, pointers, arrays and structs of comparable fields. A program cannot give a type its own hash or equality. The fails block uses a slice as a key, and the compiler rejects it because slices are not comparable; an array or a string built from the slice works. A NaN key is never equal to itself, so each insert adds an entry that no lookup finds. `hash/maphash` gives seeded hashes for programs that build their own tables.
