```go run id=three-layers
package main

import (
	"errors"
	"fmt"
	"maps"
	"slices"
	"sort"
)

type order struct {
	customer, item string
	quantity, cents int
}

type tables struct {
	stock  map[string]int
	orders []order
}

// Data source layer: the stored data, and a transaction that keeps all or none of a change.
type store struct{ t tables }

func (s *store) transaction(change func(t *tables) error) bool {
	saved := tables{stock: maps.Clone(s.t.stock), orders: slices.Clone(s.t.orders)}
	if err := change(&s.t); err != nil {
		s.t = saved // roll back
		return false
	}
	return true
}

// Domain layer: the business rules, with a special deal for one customer.
const priceCents = 1000

func price(customer string, quantity int) int {
	total := priceCents * quantity
	if customer == "acme" && quantity >= 3 {
		total = total * 90 / 100 // a deal that sales made with one customer
	}
	return total
}

func placeOrder(s *store, customer, item string, quantity int) bool {
	return s.transaction(func(t *tables) error {
		t.orders = append(t.orders, order{customer, item, quantity, price(customer, quantity)})
		if t.stock[item] < quantity {
			return errors.New("not enough stock")
		}
		t.stock[item] -= quantity
		return nil
	})
}

func money(cents int) string { return fmt.Sprintf("%d.%02d", cents/100, cents%100) }

// Presentation layer: two screens show the same data in different ways.
func orderScreen(s *store) {
	for _, o := range s.t.orders {
		fmt.Printf("order screen: %s bought %d x %s for %s\n", o.customer, o.quantity, o.item, money(o.cents))
	}
}

func stockScreen(s *store) {
	items := make([]string, 0, len(s.t.stock))
	for item := range s.t.stock {
		items = append(items, item)
	}
	sort.Strings(items) // map order is random, so the screen sorts the keys
	for _, item := range items {
		fmt.Printf("stock screen: %s: %d in stock, orders: %d\n", item, s.t.stock[item], len(s.t.orders))
	}
}

func result(ok bool) string {
	if ok {
		return "kept"
	}
	return "rolled back"
}

func main() {
	s := &store{t: tables{stock: map[string]int{"widget": 10}}}
	fmt.Println("acme orders 3:", result(placeOrder(s, "acme", "widget", 3)))
	fmt.Println("bolt orders 9:", result(placeOrder(s, "bolt", "widget", 9)))
	orderScreen(s)
	stockScreen(s)
}
```

```output
acme orders 3: kept
bolt orders 9: rolled back
order screen: acme bought 3 x widget for 27.00
stock screen: widget: 7 in stock, orders: 1
```

- Verdict: stdlib. `net/http` and `database/sql` in the standard library give the requests and a common database interface, and a driver gives each database. The example clones the map and the slice for the rollback, and the stock screen sorts the keys because map order is random.
