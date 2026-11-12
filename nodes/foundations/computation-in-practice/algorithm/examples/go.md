```go run id=slices-binarysearch
package main

import (
	"cmp"
	"fmt"
	"slices"
	"sort"
)

func main() {
	xs := []int{90, 12, 58, 31, 47}
	slices.Sort(xs) // not stable
	fmt.Println("sorted:", xs)

	for _, t := range []int{58, 50} {
		i, found := slices.BinarySearch(xs, t)
		fmt.Printf("search %d: index %d, found %v\n", t, i, found)
	}

	// sort.Search finds the first index where a monotone predicate is true.
	n := sort.Search(1000, func(k int) bool { return k*k >= 500 })
	fmt.Println("smallest k with k*k >= 500:", n)

	type person struct {
		name string
		born int
	}
	ps := []person{{"Grace", 1906}, {"Ada", 1815}, {"Alan", 1912}}
	slices.SortStableFunc(ps, func(a, b person) int { return cmp.Compare(a.born, b.born) })
	fmt.Println(ps)
}
```

```output for=slices-binarysearch
sorted: [12 31 47 58 90]
search 58: index 3, found true
search 50: index 3, found false
smallest k with k*k >= 500: 23
[{Ada 1815} {Grace 1906} {Alan 1912}]
```

- Verdict: stdlib. The `slices` package, from Go 1.21, has generic `Sort`, `SortStableFunc` and `BinarySearch`. `BinarySearch` returns two values, the position and whether the target is there, so the caller cannot mistake an insertion point for a match. `sort.Search` is more general: it finds the first index where a predicate becomes true, so it also searches a range of numbers that is not stored anywhere, here the smallest k with k squared at least 500.
