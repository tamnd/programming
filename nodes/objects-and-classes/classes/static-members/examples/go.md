```go run id=package-level-state
package main

import (
	"fmt"
	"strconv"
)

// TaxRate and created play the part of static fields: one per package.
const TaxRate = 0.2

var created int

type Order struct {
	ID     int
	amount int
}

// NewOrder and OrderFromText play the part of static methods.
func NewOrder(amount int) *Order {
	created++
	return &Order{ID: created, amount: amount}
}

func OrderFromText(text string) (*Order, error) {
	n, err := strconv.Atoi(text)
	if err != nil {
		return nil, err
	}
	return NewOrder(n), nil
}

func (o *Order) Total() float64 { return float64(o.amount) * (1 + TaxRate) }

func main() {
	a := NewOrder(100)
	b, _ := OrderFromText("50")
	fmt.Println(a.ID, b.ID, created)
	fmt.Println(a.Total())
}
```

```output for=package-level-state
1 2 2
120
```

- Verdict: idiom. Go has no static members. A package-level variable, constant or function does the job, and the package, not the type, is the unit that owns it.
- The names say which type a function belongs to: `NewOrder` and `OrderFromText`. Code in another package calls them as `orders.NewOrder(100)`.
