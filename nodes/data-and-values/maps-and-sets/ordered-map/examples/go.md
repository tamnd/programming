```go run id=sorted-keys
package main

import (
	"fmt"
	"maps"
	"slices"
	"sort"
)

func main() {
	// Go has no ordered map. Sort the keys of a map when the order matters.
	m := map[int]string{30: "c", 10: "a", 50: "e", 20: "b", 40: "d"}
	keys := slices.Sorted(maps.Keys(m))
	fmt.Println(keys)
	for _, k := range keys[:2] {
		fmt.Printf("%d=%s\n", k, m[k])
	}

	// Floor and ceiling with a binary search on the sorted keys.
	i := sort.SearchInts(keys, 35) // the first index with keys[i] >= 35
	fmt.Println(keys[i-1], keys[i])

	// BinarySearch also says whether the key is there.
	j, found := slices.BinarySearch(keys, 40)
	fmt.Println(j, found)

	// A range query: the keys from 20 up to but not including 45.
	lo, _ := slices.BinarySearch(keys, 20)
	hi, _ := slices.BinarySearch(keys, 45)
	fmt.Println(keys[lo:hi])

	// An insert into the sorted keys must keep them sorted, and it moves the later keys.
	k := 35
	p, _ := slices.BinarySearch(keys, k)
	keys = slices.Insert(keys, p, k)
	m[k] = "x"
	fmt.Println(keys)
}
```

```output
[10 20 30 40 50]
10=a
20=b
30 40
3 true
[20 30 40]
[10 20 30 35 40 50]
```

- Verdict: idiom. Go has no ordered map. slices.Sorted(maps.Keys(m)) gives the keys in order, and sort.SearchInts and slices.BinarySearch give floor, ceiling and ranges. slices.Insert keeps the keys sorted, but it moves the later keys.
