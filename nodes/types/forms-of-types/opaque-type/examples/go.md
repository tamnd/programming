```go run id=unexported
package main

import "fmt"

// Set is the exported face of the type.
type Set interface {
	Add(s string)
	Has(s string) bool
	Len() int
}

// mapSet starts with a lowercase letter, so other packages cannot name it.
type mapSet struct {
	items map[string]struct{}
}

func NewSet() Set {
	return &mapSet{items: map[string]struct{}{}}
}

func (m *mapSet) Add(s string)      { m.items[s] = struct{}{} }
func (m *mapSet) Has(s string) bool { _, ok := m.items[s]; return ok }
func (m *mapSet) Len() int          { return len(m.items) }

func main() {
	s := NewSet()
	s.Add("a")
	s.Add("a")
	s.Add("b")
	fmt.Println(s.Len(), s.Has("a"), s.Has("z"))
	fmt.Printf("%T\n", s)
}
```

```output for=unexported
2 true false
*main.mapSet
```

- Verdict: idiom. Go hides by package: `mapSet` and its field `items` start with a lowercase letter, so code in another package cannot name them, and must use `NewSet` and the methods of `Set`. In this one-file example everything is in package `main`. `%T` shows that the dynamic type is still there when the program runs.
