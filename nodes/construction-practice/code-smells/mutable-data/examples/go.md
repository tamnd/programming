```go run id=sort-a-copy
package main

import (
	"fmt"
	"slices"
)

// Before: slices.Sort changes the caller's slice.
func medianBefore(values []int) int {
	slices.Sort(values)
	return values[len(values)/2]
}

// After: sort a clone.
func median(values []int) int {
	sorted := slices.Clone(values)
	slices.Sort(sorted)
	return sorted[len(sorted)/2]
}

func main() {
	readings := []int{30, 10, 20}
	if medianBefore(readings) != 20 || !slices.Equal(readings, []int{10, 20, 30}) {
		panic("before")
	}
	fmt.Println("medianBefore: the caller's slice was changed")

	readings = []int{30, 10, 20}
	if median(readings) != 20 || !slices.Equal(readings, []int{30, 10, 20}) {
		panic("after")
	}
	fmt.Println("median: tests pass")

	// A sub-slice shares the array, so a sort of the part changes the whole.
	all := []int{30, 10, 20, 5}
	medianBefore(all[:3])
	if !slices.Equal(all, []int{10, 20, 30, 5}) {
		panic("sub-slice")
	}
	fmt.Println("medianBefore: a sub-slice changed the whole slice")
}
```

```output
medianBefore: the caller's slice was changed
median: tests pass
medianBefore: a sub-slice changed the whole slice
```

- A Go slice is a small header that points to an array. Passing a slice copies the header, not the array, so the function and the caller share the elements.
- `slices.Clone` makes a shallow copy. For a slice of integers that is a full copy. For a slice of pointers, the copy shares the values that the pointers refer to.
- The last test shows that `all[:3]` is a view of the first three elements of `all`. The sort changes them in `all` too.
