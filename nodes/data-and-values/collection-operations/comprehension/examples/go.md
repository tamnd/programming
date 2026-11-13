```go run id=loops-and-iterators
package main

import (
	"fmt"
	"maps"
	"slices"
	"strings"
)

func main() {
	words := []string{"apple", "fig", "banana", "kiwi", "cherry"}

	// Go has no comprehension. A loop with append builds the list.
	var lengths []int
	for _, w := range words {
		if len(w) > 3 {
			lengths = append(lengths, len(w))
		}
	}
	fmt.Println(lengths)

	// A map with empty struct values is a set. A loop fills a map in the same way.
	firsts := map[byte]struct{}{}
	withI := map[string]int{}
	for _, w := range words {
		firsts[w[0]] = struct{}{}
		if strings.Contains(w, "i") {
			withI[w] = len(w)
		}
	}
	fmt.Println(string(slices.Sorted(maps.Keys(firsts))), withI)

	// Two loops nest like two for clauses.
	var pairs []string
	for x := 1; x <= 2; x++ {
		for _, y := range []string{"a", "b"} {
			pairs = append(pairs, fmt.Sprint(x, y))
		}
	}
	fmt.Println(pairs)

	// An iterator function makes the items one at a time, so the loop can stop early.
	squares := func(yield func(int) bool) {
		for n := 0; ; n++ {
			if !yield(n * n) {
				return
			}
		}
	}
	var small []int
	for s := range squares {
		if s > 10 {
			break
		}
		small = append(small, s)
	}
	fmt.Println(small)
}
```

```output
[5 6 4 6]
abcfk map[fig:3 kiwi:4]
[1a 1b 2a 2b]
[0 1 4 9]
```

- Verdict: idiom. Go has no comprehension syntax. A for loop with append builds a slice, and a loop fills a map or a set made from a map. fmt.Println prints a map with its keys in sorted order. The iterator function gives squares without end, and the range loop stops it with break.
