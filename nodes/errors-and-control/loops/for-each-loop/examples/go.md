```go run id=range-copies-sorted-keys-and-runes
package main

import (
	"fmt"
	"sort"
)

type point struct{ x, y int }

func main() {
	pts := []point{{1, 2}, {3, 4}}
	for _, p := range pts {
		p.x = 100 // changes a copy
		fmt.Println("copy:", p)
	}
	fmt.Println(pts)
	for i := range pts {
		pts[i].x *= 10 // changes the element
	}
	fmt.Println(pts)

	// Map order is not fixed, so sort the keys for stable output.
	ages := map[string]int{"bob": 27, "ann": 31, "cy": 5}
	keys := make([]string, 0, len(ages))
	for k := range ages {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	for _, k := range keys {
		fmt.Println(k, ages[k])
	}

	// range over a string gives byte offsets and runes.
	for i, r := range "hé!" {
		fmt.Println(i, string(r))
	}
}
```

```output for=range-copies-sorted-keys-and-runes
copy: {100 2}
copy: {100 4}
[{1 2} {3 4}]
[{10 2} {30 4}]
ann 31
bob 27
cy 5
0 h
1 é
3 !
```

- Verdict: native. `for _, p := range pts` copies each element into `p`, so the slice keeps its values, and the loop over the index changes the elements. The keys of a map come in no fixed order, so the code sorts them. Over a string, `range` gives the byte offset and the rune, so `!` is at offset 3, after the two bytes of `é`.
