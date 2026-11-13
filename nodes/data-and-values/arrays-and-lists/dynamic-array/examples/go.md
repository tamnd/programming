```go run id=append-and-cap
package main

import "fmt"

func main() {
	// A slice has a length and a capacity. append grows the backing array when it is full.
	var s []int
	caps := []int{}
	for i := range 2000 {
		s = append(s, i)
		if len(caps) == 0 || caps[len(caps)-1] != cap(s) {
			caps = append(caps, cap(s))
		}
	}
	fmt.Println(len(s), caps)

	// make with a capacity gets the memory once. append then does not reallocate.
	t := make([]int, 0, 100)
	before := cap(t)
	for i := range 100 {
		t = append(t, i)
	}
	fmt.Println(len(t), before == cap(t))

	// Two appends to one slice with spare capacity write into the same array.
	base := make([]int, 3, 4)
	x := append(base, 1)
	y := append(base, 2)
	fmt.Println(x[3], y[3])

	// When append reallocates, the new slice has its own array.
	full := []int{1, 2, 3}
	grown := append(full, 4)
	grown[0] = 100
	fmt.Println(full[0], grown[0])
}
```

```output
2000 [4 8 16 32 64 128 256 512 848 1280 1792 2560]
100 true
2 2
1 100
```

- Verdict: native. append doubles the capacity of a small slice and grows a large slice by a smaller factor, rounded to the sizes of the allocator: 512, 848, 1280, 1792 and 2560. make with a capacity keeps the same array for 100 appends. Two appends to one slice with spare capacity share the array, so x[3] and y[3] are both 2.
