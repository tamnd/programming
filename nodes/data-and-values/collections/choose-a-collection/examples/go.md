```go run id=five-needs
package main

import (
	"container/list"
	"fmt"
	"maps"
	"slices"
	"strings"
)

func main() {
	words := strings.Fields("the cat saw the dog and the cat ran")

	seen := map[string]bool{} // a set is a map with value type bool
	counts := map[string]int{}
	for _, w := range words {
		seen[w] = true
		counts[w]++
	}
	line := list.New() // a doubly linked list as a queue
	for _, w := range words[:3] {
		line.PushBack(w)
	}
	first := line.Remove(line.Front())
	second := line.Remove(line.Front())

	fmt.Printf("list: %d words, third is %s\n", len(words), words[2])
	fmt.Printf("set: %d distinct\n", len(seen))
	fmt.Printf("map: the=%d cat=%d\n", counts["the"], counts["cat"])
	fmt.Printf("queue: %v, then %v\n", first, second)
	fmt.Println("sorted:", strings.Join(slices.Sorted(maps.Keys(counts)), " "))
}
```

```output for=five-needs
list: 9 words, third is saw
set: 6 distinct
map: the=3 cat=2
queue: the, then cat
sorted: and cat dog ran saw the
```

- Go has slices and maps as built-in types. A set is a `map[string]bool`, a queue can be a `container/list`, and there is no sorted map, so `slices.Sorted(maps.Keys(counts))` sorts the keys.
- The order of `maps.Keys` is not specified, so the keys must be sorted before they are printed.
