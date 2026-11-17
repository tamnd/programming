```go run id=constraints
package main

import (
	"cmp"
	"fmt"
)

// Number is a constraint: a union of types.
type Number interface {
	~int | ~int64 | ~float64
}

func Max[T cmp.Ordered](items []T) T {
	best := items[0]
	for _, x := range items[1:] {
		if x > best {
			best = x
		}
	}
	return best
}

func Sum[T Number](items []T) T {
	var total T
	for _, x := range items {
		total += x
	}
	return total
}

type Celsius float64

func main() {
	fmt.Println(Max([]int{3, 9, 4}), Max([]string{"pear", "apple"}))
	fmt.Println(Sum([]Celsius{1.5, 2.5}), Sum([]int64{1, 2, 3}))
}
```

```output for=constraints
9 pear
4 6
```

- Verdict: native. `cmp.Ordered` is a constraint from the standard library, so `Max` can use `>`. `Number` lists types, so `Sum` can use `+=`. The `~` lets `Celsius`, whose underlying type is `float64`, meet the constraint.
