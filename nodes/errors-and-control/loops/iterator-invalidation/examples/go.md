```go run id=delete-in-map-range-and-append-in-slice-range
package main

import (
	"fmt"
	"slices"
	"sort"
)

func main() {
	// Deleting map entries during range is defined and safe.
	stock := map[string]int{"pen": 0, "ink": 5, "pad": 0, "cap": 2}
	for k, v := range stock {
		if v == 0 {
			delete(stock, k)
		}
	}
	keys := make([]string, 0, len(stock))
	for k := range stock {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	fmt.Println(keys)

	// The range expression is evaluated once, so append does not extend the loop.
	s := []int{1, 2, 3}
	passes := 0
	for _, x := range s {
		s = append(s, x*10)
		passes++
	}
	fmt.Println(passes, s)

	// slices.DeleteFunc removes all matches in one call.
	nums := []int{1, 2, 2, 3, 4}
	nums = slices.DeleteFunc(nums, func(n int) bool { return n%2 == 0 })
	fmt.Println(nums)
}
```

```output for=delete-in-map-range-and-append-in-slice-range
[cap ink]
3 [1 2 3 10 20 30]
[1 3]
```

- Verdict: native. The Go specification defines a deletion during a `range` over a map: an entry that is deleted before the loop reaches it is not produced. The `range` over the slice uses the slice that it evaluated at the start, so the three appends do not add passes. `slices.DeleteFunc` removes the even numbers and returns the shorter slice.
