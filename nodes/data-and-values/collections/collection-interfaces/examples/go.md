```go run id=to-the-interface
package main

import (
	"fmt"
	"iter"
	"maps"
	"slices"
)

func total(xs iter.Seq[int]) int {
	sum := 0
	for x := range xs {
		sum += x
	}
	return sum
}

func describe(size int, xs iter.Seq[int]) string {
	return fmt.Sprintf("size %d, total %d", size, total(xs))
}

func main() {
	list := []int{1, 2, 3}
	set := map[int]struct{}{4: {}, 5: {}}
	prices := map[string]int{"tea": 3, "cake": 4}

	fmt.Println("list:", describe(len(list), slices.Values(list)))
	fmt.Println("set:", describe(len(set), maps.Keys(set)))
	fmt.Println("map values:", describe(len(prices), maps.Values(prices)))
}
```

```output for=to-the-interface
list: size 3, total 6
set: size 2, total 9
map values: size 2, total 7
```

- Verdict: stdlib. Go has no collection interfaces. `iter.Seq[int]` is a function type for any sequence of `int` values, and `slices.Values`, `maps.Keys` and `maps.Values` make one from a built-in collection.
- A `Seq` has no size, and `len` works only on built-in types, so `describe` takes the size as a second argument.
