```go run id=map-a-customer
package main

import (
	"fmt"
	"strings"
)

type customerRow struct {
	id                 int
	name, street, city string
}

type orderRow struct {
	id, customerID, total int
	kind, card            string
}

// The tables.
var customerTable = []customerRow{{1, "Ann", "Main St 1", "Oslo"}, {2, "Bo", "High St 9", "Rome"}}
var orderTable = []orderRow{{10, 1, 30, "card", "4242"}, {11, 1, 20, "cash", ""}, {12, 2, 45, "card", "1881"}}
var statements = 0

// Each function stands in for one SQL statement.
func selectCustomer(id int) customerRow {
	statements++
	for _, r := range customerTable {
		if r.id == id {
			return r
		}
	}
	return customerRow{}
}

func selectOrders(customerID int) []orderRow {
	statements++
	var out []orderRow
	for _, r := range orderTable {
		if r.customerID == customerID {
			out = append(out, r)
		}
	}
	return out
}

func selectCustomerIDs() []int {
	statements++
	var out []int
	for _, r := range customerTable {
		out = append(out, r.id)
	}
	return out
}

func joinTotals() []string { // select name, sum(total) ... group by
	statements++
	var out []string
	for _, c := range customerTable {
		sum := 0
		for _, o := range orderTable {
			if o.customerID == c.id {
				sum += o.total
			}
		}
		out = append(out, fmt.Sprintf("%s %d", c.name, sum))
	}
	return out
}

// Address is a value object: it has no table and no key.
type Address struct{ Street, City string }

type Customer struct {
	ID      int
	Name    string
	Address Address
	Orders  []Order
}

// Order is an interface, because Go has no subclasses.
type Order interface {
	Base() *OrderBase
}

type OrderBase struct {
	ID, Total int
	Customer  *Customer
}

type CardOrder struct {
	OrderBase
	Card string
}

type CashOrder struct{ OrderBase }

func (o *CardOrder) Base() *OrderBase { return &o.OrderBase }
func (o *CashOrder) Base() *OrderBase { return &o.OrderBase }

func load(id int) *Customer {
	r := selectCustomer(id)
	// Granularity: one row becomes two values.
	c := &Customer{ID: id, Name: r.name, Address: Address{r.street, r.city}}
	for _, o := range selectOrders(id) {
		// Subtypes: a column chooses the type.
		base := OrderBase{o.id, o.total, c}
		var order Order = &CashOrder{base}
		if o.kind == "card" {
			order = &CardOrder{base, o.card}
		}
		// Associations: the foreign key becomes two references.
		c.Orders = append(c.Orders, order)
	}
	return c
}

func lists(c *Customer, o Order) string {
	for _, x := range c.Orders {
		if x == o {
			return "yes"
		}
	}
	return "no"
}

func yn(flag bool) string {
	if flag {
		return "yes"
	}
	return "no"
}

func main() {
	ann, bo := load(1), load(2)
	fmt.Printf("granularity: customer row 1 -> Customer %s + Address %s, %s\n", ann.Name, ann.Address.Street, ann.Address.City)
	var kinds, ids []string
	for _, o := range ann.Orders {
		name := "CashOrder"
		if _, ok := o.(*CardOrder); ok {
			name = "CardOrder"
		}
		kinds = append(kinds, fmt.Sprintf("order %d %s", o.Base().ID, name))
		ids = append(ids, fmt.Sprint(o.Base().ID))
	}
	fmt.Printf("subtypes: %s | card %s\n", strings.Join(kinds, " | "), ann.Orders[0].(*CardOrder).Card)
	fmt.Printf("associations: order 10 -> %s | Ann -> orders %s\n", ann.Orders[0].Base().Customer.Name, strings.Join(ids, ", "))

	// In the table, one update moves the order. In memory, both sides must change.
	moved := ann.Orders[1]
	moved.Base().Customer = bo
	fmt.Printf("moved order 11 on one side: Ann lists 11: %s | Bo lists 11: %s\n", lists(ann, moved), lists(bo, moved))

	again := load(1)
	fmt.Printf("identity: same object: %s | same key: %s\n", yn(again == ann), yn(again.ID == ann.ID))

	statements = 0
	var walk []string
	for _, id := range selectCustomerIDs() {
		c := load(id)
		sum := 0
		for _, o := range c.Orders {
			sum += o.Base().Total
		}
		walk = append(walk, fmt.Sprintf("%s %d", c.Name, sum))
	}
	fmt.Printf("navigation: walk %s in %d statements\n", strings.Join(walk, ", "), statements)

	statements = 0
	join := joinTotals()
	fmt.Printf("navigation: join %s in %d statement\n", strings.Join(join, ", "), statements)
}
```

```output
granularity: customer row 1 -> Customer Ann + Address Main St 1, Oslo
subtypes: order 10 CardOrder | order 11 CashOrder | card 4242
associations: order 10 -> Ann | Ann -> orders 10, 11
moved order 11 on one side: Ann lists 11: yes | Bo lists 11: no
identity: same object: no | same key: yes
navigation: walk Ann 50, Bo 45 in 5 statements
navigation: join Ann 50, Bo 45 in 1 statement
```

Go has no type hierarchy, so `Order` is an interface, and `CardOrder` and `CashOrder` embed `OrderBase`. Two slices stand in for the tables, and each `select` function counts as one SQL statement. `load` returns a `*Customer`, and `==` on two pointers is true only if they point to the same variable, so the second `load(1)` is not the same customer.
