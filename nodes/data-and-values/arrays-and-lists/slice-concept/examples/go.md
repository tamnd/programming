```go run id=shared-array
package main

import (
	"fmt"
	"slices"
)

func main() {
	// A slice expression makes a new slice header over the same array. No item is copied.
	arr := [5]int{10, 20, 30, 40, 50}
	s := arr[1:3]
	s[0] = 99
	fmt.Println(s, len(s), cap(s), arr)

	// The capacity reaches to the end of the array, so append writes into arr.
	s = append(s, 77)
	fmt.Println(s, arr)

	// A full slice expression a[low:high:max] limits the capacity.
	// The next append must copy, so arr does not change.
	t := arr[1:3:3]
	t = append(t, 66)
	fmt.Println(t, cap(t) >= 3, arr)

	// A slice can be resliced up to its capacity, past its length.
	u := arr[0:1]
	fmt.Println(u, u[:4])

	// slices.Clone and copy make independent slices.
	c := slices.Clone(arr[:2])
	c[0] = 1
	dst := make([]int, 2)
	n := copy(dst, arr[3:])
	fmt.Println(c, dst, n, arr[0])
}
```

```output
[99 30] 2 4 [10 99 30 40 50]
[99 30 77] [10 99 30 77 50]
[99 30 66] true [10 99 30 77 50]
[10] [10 99 30 77]
[1 99] [77 50] 2 10
```

- Verdict: native. arr[1:3] shares the array, so a write through the slice changes arr. Its capacity reaches the end of arr, so append writes 77 into arr. The full slice expression arr[1:3:3] limits the capacity, so the next append copies. A slice can be resliced past its length up to its capacity. slices.Clone and copy make independent slices.
