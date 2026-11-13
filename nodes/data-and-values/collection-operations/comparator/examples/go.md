```go run id=sortfunc-and-cmp-or
package main

import (
	"cmp"
	"fmt"
	"math"
	"slices"
	"strings"
)

type person struct {
	name string
	age  int
}

func main() {
	// cmp.Compare returns -1, 0 or +1.
	fmt.Println(cmp.Compare(1, 2), cmp.Compare(2, 2), cmp.Compare("b", "a"))

	people := []person{{"cy", 31}, {"ann", 31}, {"bob", 25}}

	// slices.SortFunc takes a three-way compare function; cmp.Or chains the keys.
	slices.SortFunc(people, func(a, b person) int {
		return cmp.Or(cmp.Compare(a.age, b.age), strings.Compare(a.name, b.name))
	})
	fmt.Println(people)

	// Swap a and b to sort in descending order.
	slices.SortFunc(people, func(a, b person) int { return cmp.Compare(b.age, a.age) })
	fmt.Println(people[0].age, people[2].age)

	// cmp.Compare puts NaN before other numbers, so floats sort in a total order.
	fs := []float64{2, math.NaN(), 1}
	slices.SortFunc(fs, cmp.Compare[float64])
	fmt.Println(fs)

	// SortStableFunc keeps equal items in their old order.
	words := []string{"bb", "a", "cc", "d"}
	slices.SortStableFunc(words, func(a, b string) int { return cmp.Compare(len(a), len(b)) })
	fmt.Println(words)
}
```

```output
-1 0 1
[{bob 25} {ann 31} {cy 31}]
31 25
[NaN 1 2]
[a d bb cc]
```

- Verdict: native. `cmp.Compare` returns -1, 0 or +1. `slices.SortFunc` takes a three-way function, and `cmp.Or` uses the name when the ages are equal. Swapped arguments sort in descending order. `cmp.Compare` puts `NaN` before the other floats. `slices.SortStableFunc` keeps `bb` before `cc` and `a` before `d`.
