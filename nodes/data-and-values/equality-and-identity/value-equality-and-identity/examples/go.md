```go run id=points
package main

import (
	"fmt"
	"math"
	"slices"
)

type Point struct{ X, Y int }

func main() {
	a := Point{1, 2}
	b := Point{1, 2}
	fmt.Println("a == b:", a == b)

	pa, pb, pc := &a, &b, &a
	fmt.Println("pa == pb:", pa == pb)
	fmt.Println("pa == pc:", pa == pc)

	counts := map[Point]int{}
	counts[a]++
	counts[b]++
	fmt.Println("distinct points:", len(counts), "count of a:", counts[a])

	xs := []int{1, 2}
	ys := []int{1, 2}
	fmt.Println("slices.Equal(xs, ys):", slices.Equal(xs, ys))

	var e1, e2 any = Point{1, 2}, Point{1, 2}
	fmt.Println("e1 == e2:", e1 == e2)

	nan := math.NaN()
	fmt.Println("nan == nan:", nan == nan)
}
```

```output for=points
a == b: true
pa == pb: false
pa == pc: true
distinct points: 1 count of a: 2
slices.Equal(xs, ys): true
e1 == e2: true
nan == nan: false
```

- Verdict: native. `==` compares structs field by field and arrays element by element. Pointers are equal when they point to the same variable. So `==` on a value is value equality, and `==` on pointers is identity.
- A comparable struct can be a map key with no extra code.
- Slices, maps and functions are not comparable, except with `nil`. `slices.Equal` and `maps.Equal` compare contents.
- Two interface values are equal when their dynamic types are identical and their dynamic values are equal.

```go fails expect=exit:2 id=uncomparable
package main

import "fmt"

func main() {
	var x, y any = []int{1}, []int{1}
	fmt.Println(x == y)
}
```

The program compiles, because interface values are comparable. At run time both dynamic types are `[]int`, which is not comparable, so the comparison panics.
