```go run id=one-object-for-all-orders
package main

import (
	"errors"
	"fmt"
	"strings"
)

type moduleError struct{ msg string }

func (e *moduleError) Error() string { return e.msg }

func cents(n int) string { return fmt.Sprintf("%d.%02d", n/100, n%100) }

// The rows of the record set. In database/sql, rows.Scan would fill them.
type customerRow struct {
	id         int
	name, tier string
}
type orderRow struct{ id, customerID int }
type orderLineRow struct {
	orderID    int
	item       string
	qty, price int
}

// RecordSet is an in-memory copy of the rows of several tables.
type RecordSet struct {
	customer  []customerRow
	orders    []orderRow
	orderLine []orderLineRow
}

func loadRecordSet() *RecordSet {
	return &RecordSet{
		customer: []customerRow{{1, "Ann", "gold"}, {2, "Bob", "regular"}, {3, "Cy", "staff"}},
		orders:   []orderRow{{1, 1}, {2, 2}, {3, 3}},
		orderLine: []orderLineRow{
			{1, "lamp", 2, 2500}, {1, "desk", 1, 7500},
			{2, "lamp", 1, 2500}, {2, "pen", 3, 500},
			{3, "lamp", 1, 2500}, {3, "pen", 3, 500},
		},
	}
}

// CustomerModule is the table module of the customer table: one value for all its rows.
type CustomerModule struct{ rows []customerRow }

func (m CustomerModule) row(id int) (customerRow, error) {
	for _, r := range m.rows {
		if r.id == id {
			return r, nil
		}
	}
	return customerRow{}, &moduleError{fmt.Sprintf("no customer %d", id)}
}

func (m CustomerModule) NamesWithTier(tier string) []string {
	var names []string
	for _, r := range m.rows {
		if r.tier == tier {
			names = append(names, r.name)
		}
	}
	return names
}

// OrderModule is the table module of the orders table.
type OrderModule struct {
	rows      []orderRow
	lines     []orderLineRow
	customers CustomerModule
}

func (m OrderModule) row(id int) (orderRow, error) {
	for _, r := range m.rows {
		if r.id == id {
			return r, nil
		}
	}
	return orderRow{}, &moduleError{fmt.Sprintf("no order %d", id)}
}

func (m OrderModule) Subtotal(id int) (int, error) {
	if _, err := m.row(id); err != nil {
		return 0, err
	}
	sum := 0
	for _, l := range m.lines {
		if l.orderID == id {
			sum += l.qty * l.price
		}
	}
	return sum, nil
}

func (m OrderModule) Total(id int) (int, error) {
	subtotal, err := m.Subtotal(id)
	if err != nil {
		return 0, err
	}
	o, _ := m.row(id)
	c, err := m.customers.row(o.customerID)
	if err != nil {
		return 0, err
	}
	switch {
	case c.tier == "gold" && subtotal >= 10000:
		return subtotal - subtotal/10, nil
	case c.tier == "staff":
		return subtotal - subtotal/5, nil
	}
	return subtotal, nil
}

func (m OrderModule) CountOver(limit int) int {
	n := 0
	for _, r := range m.rows {
		if t, _ := m.Total(r.id); t > limit {
			n++
		}
	}
	return n
}

func main() {
	rs := loadRecordSet()
	customers := CustomerModule{rs.customer}
	orders := OrderModule{rs.orders, rs.orderLine, customers}
	fmt.Printf("record set: %d customers, %d orders, %d order lines\n", len(rs.customer), len(rs.orders), len(rs.orderLine))
	for _, id := range []int{1, 2, 3} {
		s, _ := orders.Subtotal(id)
		t, _ := orders.Total(id)
		fmt.Printf("order %d: subtotal %s, total %s\n", id, cents(s), cents(t))
	}
	fmt.Println("gold customers:", strings.Join(customers.NamesWithTier("gold"), ", "))
	fmt.Println("orders over 35.00:", orders.CountOver(3500))
	var me *moduleError
	if _, err := orders.Total(9); errors.As(err, &me) {
		fmt.Println("error:", me)
	}
}
```

```output
record set: 3 customers, 3 orders, 6 order lines
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
order 3: subtotal 40.00, total 32.00
gold customers: Ann
orders over 35.00: 2
error: no order 9
```

- Verdict: idiom. Each module is a value type with a slice of the rows of its table, and its methods return an error when the record set has no row with the id. The discount rule is a `switch` on the tier.
