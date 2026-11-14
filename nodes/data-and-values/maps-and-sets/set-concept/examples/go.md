```go run id=map-as-set
package main

import (
	"fmt"
	"maps"
	"slices"
)

// Go has no set type. A map with struct{} values is a set, and struct{} takes no memory.
type Set[T comparable] map[T]struct{}

func (s Set[T]) Add(v T)      { s[v] = struct{}{} }
func (s Set[T]) Has(v T) bool { _, ok := s[v]; return ok }
func Of[T comparable](vs ...T) Set[T] {
	s := Set[T]{}
	for _, v := range vs {
		s.Add(v)
	}
	return s
}

func main() {
	s := Of(3, 1, 3, 2, 1)
	fmt.Println(len(s), s.Has(2), s.Has(5))

	// A map of bool also works, because a missing key gives false.
	seen := map[string]bool{}
	for _, w := range []string{"b", "a", "b"} {
		if seen[w] {
			fmt.Println("repeat:", w)
		}
		seen[w] = true
	}

	// Set algebra is a loop. Sort the keys to print them, because map order is random.
	a, b := Of(1, 2, 3), Of(2, 3, 4)
	inter := Set[int]{}
	for v := range a {
		if b.Has(v) {
			inter.Add(v)
		}
	}
	union := maps.Clone(a)
	maps.Copy(union, b)
	fmt.Println(slices.Sorted(maps.Keys(union)), slices.Sorted(maps.Keys(inter)))

	// delete removes an element, and deleting a missing element does nothing.
	delete(s, 1)
	delete(s, 9)
	fmt.Println(slices.Sorted(maps.Keys(s)))
}
```

```output
3 true false
repeat: b
[1 2 3 4] [2 3]
[2 3]
```

- Verdict: idiom. Go has no set type. A map[T]struct{} is a set with no memory for the values, and a map[T]bool also works because a missing key gives false. Set algebra is a loop, and the order of a loop over a map is not defined, so the example sorts the keys.
