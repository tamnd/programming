```go run id=pair
package main

import (
	"fmt"
	"slices"
)

type pair struct {
	lo, hi int
}

func minMax(values []int) (int, int) {
	return slices.Min(values), slices.Max(values)
}

func yn(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	lo, hi := minMax([]int{7, 2, 9, 4})
	fmt.Println("lo:", lo, "hi:", hi)

	p := pair{lo, hi}
	fmt.Println("pair:", p)
	fmt.Println("second part:", p.hi)
	fmt.Println("equal pairs:", yn(p == pair{2, 9}))
}
```

```output for=pair
lo: 2 hi: 9
pair: {2 9}
second part: 9
equal pairs: yes
```

- Verdict: idiom. Go has no tuple type. A function returns several values, and the caller assigns them to separate variables.
- To keep the parts together, the program declares a small struct.
- Structs with comparable fields can be compared with `==`.
