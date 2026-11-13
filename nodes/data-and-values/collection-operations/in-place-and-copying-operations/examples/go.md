```go run id=sort-clone-and-share
package main

import (
	"fmt"
	"maps"
	"slices"
	"sort"
)

func main() {
	// slices.Sort sorts the slice in place.
	nums := []int{3, 1, 2}
	slices.Sort(nums)
	fmt.Println(nums)

	// slices.Sorted collects an iterator into a new sorted slice.
	src := []int{3, 1, 2}
	fresh := slices.Sorted(slices.Values(src))
	fmt.Println(src, fresh)

	// To sort a copy of a slice, clone it first.
	copied := slices.Clone(src)
	slices.Sort(copied)
	fmt.Println(src, copied)

	// The keys of a map come out in a new slice.
	ages := map[string]int{"b": 2, "a": 1, "c": 3}
	fmt.Println(slices.Sorted(maps.Keys(ages)))

	// A subslice shares its array, so sorting it changes the whole slice.
	all := []int{9, 8, 7, 6}
	sort.Ints(all[:2])
	fmt.Println(all)

	// append can write into the shared array too.
	base := []int{1, 2, 3}
	head := base[:2]
	head = append(head, 99)
	fmt.Println(base, head)
}
```

```output
[1 2 3]
[3 1 2] [1 2 3]
[3 1 2] [1 2 3]
[a b c]
[8 9 7 6]
[1 2 99] [1 2 99]
```

- Verdict: native. `slices.Sort` changes the slice. `slices.Sorted` collects an iterator into a new slice, and `slices.Clone` makes a copy to sort. A subslice shares the array of its slice, so `sort.Ints(all[:2])` changes `all`, and `append` on `base[:2]` writes `99` into `base`.
