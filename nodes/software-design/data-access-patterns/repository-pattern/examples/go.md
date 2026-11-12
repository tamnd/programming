```go run id=orders
package main

import (
	"errors"
	"fmt"
	"slices"
	"strings"
)

type Order struct {
	ID       int
	Customer string
	Total    int
	Status   string
}

func (o *Order) Ship() error {
	if o.Status != "open" {
		return fmt.Errorf("order %d is %s", o.ID, o.Status)
	}
	o.Status = "shipped"
	return nil
}

var ErrNotFound = errors.New("not found")

// OrderRepository is declared by the code that uses it.
type OrderRepository interface {
	Add(o Order) error
	Get(id int) (Order, error)
	ByCustomer(customer string) []Order
	Update(o Order) error
}

// MemoryOrders stores Order values, so every Get returns a copy.
type MemoryOrders struct{ rows map[int]Order }

func NewMemoryOrders() *MemoryOrders { return &MemoryOrders{rows: map[int]Order{}} }

func (m *MemoryOrders) Add(o Order) error {
	if _, ok := m.rows[o.ID]; ok {
		return fmt.Errorf("order %d exists", o.ID)
	}
	m.rows[o.ID] = o
	return nil
}

func (m *MemoryOrders) Get(id int) (Order, error) {
	o, ok := m.rows[id]
	if !ok {
		return Order{}, ErrNotFound
	}
	return o, nil
}

func (m *MemoryOrders) ByCustomer(customer string) []Order {
	var out []Order
	for _, o := range m.rows {
		if o.Customer == customer {
			out = append(out, o)
		}
	}
	slices.SortFunc(out, func(a, b Order) int { return a.ID - b.ID })
	return out
}

func (m *MemoryOrders) Update(o Order) error {
	if _, ok := m.rows[o.ID]; !ok {
		return ErrNotFound
	}
	m.rows[o.ID] = o
	return nil
}

func ShipOrder(orders OrderRepository, id int) string {
	o, err := orders.Get(id)
	if errors.Is(err, ErrNotFound) {
		return fmt.Sprintf("order %d: not found", id)
	}
	if err := o.Ship(); err != nil {
		return err.Error()
	}
	if err := orders.Update(o); err != nil {
		return err.Error()
	}
	return fmt.Sprintf("order %d is %s", id, o.Status)
}

func main() {
	var repo OrderRepository = NewMemoryOrders()
	for _, o := range []Order{{1, "ana", 30, "open"}, {2, "bo", 12, "open"}, {3, "ana", 5, "open"}} {
		repo.Add(o)
	}
	var parts []string
	for _, o := range repo.ByCustomer("ana") {
		parts = append(parts, fmt.Sprintf("order %d (%d)", o.ID, o.Total))
	}
	fmt.Println("ana:", strings.Join(parts, ", "))
	fmt.Println(ShipOrder(repo, 1))
	fmt.Println(ShipOrder(repo, 9))
	stored, _ := repo.Get(1)
	fmt.Printf("stored: order %d is %s\n", stored.ID, stored.Status)
}
```

```output
ana: order 1 (30), order 3 (5)
order 1 is shipped
order 9: not found
stored: order 1 is shipped
```

- Verdict: idiom. An interface lists the operations, and a sentinel error `ErrNotFound` lets callers tell "not found" from a failure of the storage.
- The map holds `Order` values, not pointers. `Get` returns a copy, so a change to an order is lost unless the caller passes it to `Update`, as with a database.
- The order of a Go map is not specified, so `ByCustomer` sorts its result by id.
- `database/sql` gives the SQL side of a Go repository: a second type with the same methods would hold a `*sql.DB`. The standard library has no driver for a database, so this program stays in memory.
