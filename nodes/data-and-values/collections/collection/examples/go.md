```go run id=three-kinds
package main

import (
	"fmt"
	"maps"
	"slices"
)

func main() {
	words := []string{"red", "blue", "red", "green"}
	unique := map[string]struct{}{}
	counts := map[string]int{}
	for _, w := range words {
		unique[w] = struct{}{}
		counts[w]++
	}

	_, hasRed := unique["red"]
	fmt.Println(len(words), words[1], slices.Contains(words, "red"))
	fmt.Println(len(unique), hasRed)
	fmt.Println(len(counts), counts["red"], slices.Sorted(maps.Keys(counts)))
}
```

```output for=three-kinds
4 blue true
3 true
3 2 [blue green red]
```

- Verdict: native. Arrays, slices and maps are built into the language. Go has no set type, so the code uses a map with empty values as a set.
- The order of a loop over a map is not fixed, so the code sorts the keys before it prints them.
- The packages `container/list`, `container/heap` and `container/ring` have other collections.
