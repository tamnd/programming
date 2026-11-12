```go run id=no-stale-copy
package main

import "fmt"

type Item struct {
	Name  string
	Cents int
}

// Cart keeps only its items; the total is computed when asked.
type Cart struct{ items []Item }

func (c *Cart) Add(name string, cents int) { c.items = append(c.items, Item{name, cents}) }

func (c *Cart) Remove(name string) {
	kept := c.items[:0]
	for _, it := range c.items {
		if it.Name != name {
			kept = append(kept, it)
		}
	}
	c.items = kept
}

func (c *Cart) TotalCents() int {
	total := 0
	for _, it := range c.items {
		total += it.Cents
	}
	return total
}

func main() {
	var c Cart
	c.Add("tea", 350)
	c.Add("cake", 400)
	c.Remove("cake")
	if c.TotalCents() != 350 {
		panic(c.TotalCents())
	}
	fmt.Println("replace derived variable with query: tests pass")
}
```

```output
replace derived variable with query: tests pass
```
