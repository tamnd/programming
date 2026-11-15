```go run id=one-service-two-clients
package main

import (
	"errors"
	"fmt"
	"maps"
)

type domainError struct{ msg string }

func (e *domainError) Error() string { return e.msg }

func fail(format string, args ...any) error { return &domainError{fmt.Sprintf(format, args...)} }

func cents(n int) string { return fmt.Sprintf("%d.%02d", n/100, n%100) }

type customer struct{ name, email, tier string }

type line struct {
	item       string
	qty, price int
}

type item struct {
	name string
	qty  int
}

// Order is the domain layer: it holds the business rules.
type Order struct {
	number   int
	customer customer
	lines    []line
}

func (o *Order) Add(name string, qty, price int) error {
	if qty <= 0 {
		return fail("bad quantity %d", qty)
	}
	o.lines = append(o.lines, line{name, qty, price})
	return nil
}

func (o *Order) Total() int {
	subtotal := 0
	for _, l := range o.lines {
		subtotal += l.qty * l.price
	}
	if o.customer.tier == "gold" && subtotal >= 10000 {
		return subtotal - subtotal/10
	}
	return subtotal
}

// Store is the data source, which the service layer coordinates.
type Store struct {
	customers map[int]customer
	prices    map[string]int
	orders    map[int]*Order
	saved     map[int]*Order
}

func (s *Store) Begin()    { s.saved = maps.Clone(s.orders) } // a copy of the map of orders
func (s *Store) Commit()   { s.saved = nil }
func (s *Store) Rollback() { s.orders = s.saved }

type Mailer struct{ sent []string }

func (m *Mailer) Send(to, text string) { m.sent = append(m.sent, to+": "+text) }

// OrderService is the service layer: the operations of the application, with the
// transaction and the coordination of the responses in one place.
type OrderService struct {
	store  *Store
	mailer *Mailer
}

func (s *OrderService) PlaceOrder(customerID int, items []item) (*Order, error) {
	s.store.Begin()
	order, err := s.build(customerID, items)
	if err != nil {
		s.store.Rollback()
		return nil, err
	}
	s.store.Commit()
	// Only a committed order gets an email.
	s.mailer.Send(order.customer.email, fmt.Sprintf("order %d, total %s", order.number, cents(order.Total())))
	return order, nil
}

func (s *OrderService) build(customerID int, items []item) (*Order, error) {
	c, ok := s.store.customers[customerID]
	if !ok {
		return nil, fail("no customer %d", customerID)
	}
	order := &Order{number: len(s.store.orders) + 1, customer: c}
	s.store.orders[order.number] = order
	for _, it := range items {
		price, ok := s.store.prices[it.name]
		if !ok {
			return nil, fail("no product %s", it.name)
		}
		if err := order.Add(it.name, it.qty, price); err != nil {
			return nil, err
		}
	}
	return order, nil
}

func main() {
	store := &Store{
		customers: map[int]customer{1: {"Ann", "ann@example.com", "gold"}, 2: {"Bob", "bob@example.com", "regular"}},
		prices:    map[string]int{"lamp": 2500, "desk": 7500, "pen": 500},
		orders:    map[int]*Order{},
	}
	mailer := &Mailer{}
	service := &OrderService{store, mailer}

	// Client 1: a web handler.
	o, _ := service.PlaceOrder(1, []item{{"lamp", 2}, {"desk", 1}})
	fmt.Printf("web: placed order %d for %s, total %s\n", o.number, o.customer.name, cents(o.Total()))

	// Client 2: a loader that imports orders from a file.
	rows := [][]item{{{"lamp", 1}, {"pen", 3}}, {{"pen", 1}, {"chair", 1}}}
	for i, items := range rows {
		o, err := service.PlaceOrder(2, items)
		var de *domainError
		switch {
		case errors.As(err, &de):
			fmt.Printf("import line %d: refused: %s\n", i+1, de)
		case err == nil:
			fmt.Printf("import line %d: placed order %d for %s, total %s\n", i+1, o.number, o.customer.name, cents(o.Total()))
		}
	}

	fmt.Println("orders stored:", len(store.orders))
	for _, m := range mailer.sent {
		fmt.Println("email", m)
	}
}
```

```output
web: placed order 1 for Ann, total 112.50
import line 1: placed order 2 for Bob, total 40.00
import line 2: refused: no product chair
orders stored: 2
email ann@example.com: order 1, total 112.50
email bob@example.com: order 2, total 40.00
```

- Verdict: idiom. `OrderService` is a struct with one method for each operation. `PlaceOrder` calls `build` for the work and decides from its error whether to commit or roll back. In a Go service, `build` would take a `*sql.Tx`.
- `domainError` marks the errors that the user can correct, and the import client finds them with `errors.As`. `maps.Clone` makes the copy of the map of orders.
