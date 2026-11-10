```go run id=checkout
package main

import "fmt"

type Item struct {
	Name     string
	Price    int // in cents
	Quantity int
}

// Pricing is the strategy: any function with this signature.
type Pricing func(items []Item) int

type Checkout struct {
	Pricing Pricing
}

func (c *Checkout) Total(items []Item) int {
	return c.Pricing(items)
}

func regular(items []Item) int {
	total := 0
	for _, item := range items {
		total += item.Price * item.Quantity
	}
	return total
}

func tenPercentOff(items []Item) int {
	return regular(items) * 90 / 100
}

// bulk takes 50 cents off each unit of a line with 5 or more units.
func bulk(items []Item) int {
	total := 0
	for _, item := range items {
		price := item.Price
		if item.Quantity >= 5 {
			price -= 50
		}
		total += price * item.Quantity
	}
	return total
}

func main() {
	cart := []Item{{"book", 1500, 2}, {"pen", 200, 5}}
	strategies := []struct {
		name    string
		pricing Pricing
	}{
		{"regular", regular},
		{"ten percent off", tenPercentOff},
		{"bulk", bulk},
	}
	checkout := &Checkout{Pricing: regular}
	for _, s := range strategies {
		checkout.Pricing = s.pricing
		fmt.Printf("%s: %d cents\n", s.name, checkout.Total(cart))
	}
}
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. Go functions are values, so a named func type is the strategy interface. `slices.SortFunc` and `sort.Slice` take their comparison in the same way.
- A one-method interface such as `type Pricer interface { Total([]Item) int }` is the class form. A func type can satisfy it through a method, as `http.HandlerFunc` satisfies `http.Handler`.
- A nil `Pricing` field makes `Total` panic. A constructor that requires the strategy, or a default in `Total`, prevents this.
