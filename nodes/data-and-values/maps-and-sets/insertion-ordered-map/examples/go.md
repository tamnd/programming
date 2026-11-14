```go run id=keys-slice
package main

import (
	"fmt"
	"slices"
	"strings"
)

// Go maps do not keep any order, so this type keeps a slice of keys next to the map.
type Ordered struct {
	keys []string
	vals map[string]int
}

func NewOrdered() *Ordered { return &Ordered{vals: map[string]int{}} }

func (o *Ordered) Set(k string, v int) {
	if _, ok := o.vals[k]; !ok {
		o.keys = append(o.keys, k)
	}
	o.vals[k] = v
}

// Delete must also remove the key from the slice. This is linear.
func (o *Ordered) Delete(k string) {
	if _, ok := o.vals[k]; !ok {
		return
	}
	delete(o.vals, k)
	o.keys = slices.DeleteFunc(o.keys, func(s string) bool { return s == k })
}

func (o *Ordered) String() string {
	parts := make([]string, 0, len(o.keys))
	for _, k := range o.keys {
		parts = append(parts, fmt.Sprintf("%s=%d", k, o.vals[k]))
	}
	return strings.Join(parts, " ")
}

func main() {
	o := NewOrdered()
	o.Set("pear", 4)
	o.Set("apple", 7)
	o.Set("fig", 2)
	fmt.Println(o)
	o.Set("pear", 9)
	fmt.Println(o)
	o.Delete("pear")
	o.Set("pear", 1)
	fmt.Println(o)

	// A plain map: range gives the keys in an order that is not specified.
	// Sorting is one way to get a stable order; insertion order is lost.
	m := map[string]int{"pear": 4, "apple": 7, "fig": 2}
	seen := []string{}
	for k := range m {
		seen = append(seen, k)
	}
	slices.Sort(seen)
	fmt.Println(seen, len(m))
}
```

```output
pear=4 apple=7 fig=2
pear=9 apple=7 fig=2
apple=7 fig=2 pear=1
[apple fig pear] 3
```

- Verdict: idiom. A Go map has no order, and a range loop gives the keys in an order that is not specified. The example keeps a slice of keys next to the map. Delete must also remove the key from the slice, which is linear.
