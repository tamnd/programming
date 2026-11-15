```go run id=orders-that-know-their-rules
package main

import (
	"errors"
	"fmt"
)

type domainError struct{ msg string }

func (e *domainError) Error() string { return e.msg }

func fail(format string, args ...any) error { return &domainError{fmt.Sprintf(format, args...)} }

func cents(n int) string { return fmt.Sprintf("%d.%02d", n/100, n%100) }

// Customer is the interface of each kind of customer. Each kind knows its own discount rule.
type Customer interface {
	Name() string
	Kind() string
	Discount(subtotal int) int
}

type person struct{ name string }

func (p person) Name() string { return p.name }

type regular struct{ person }

func (regular) Kind() string       { return "regular" }
func (regular) Discount(_ int) int { return 0 }

type gold struct{ person }

func (gold) Kind() string { return "gold" }
func (gold) Discount(subtotal int) int {
	if subtotal >= 10000 {
		return subtotal / 10
	}
	return 0
}

type staff struct{ person }

func (staff) Kind() string              { return "staff" }
func (staff) Discount(subtotal int) int { return subtotal / 5 }

type Product struct {
	Name       string
	PriceCents int
}

type orderLine struct {
	product Product
	qty     int
}

// Order keeps its lines and its state private, so only its methods can change them.
type Order struct {
	Number   int
	Customer Customer
	lines    []orderLine
	placed   bool
}

func (o *Order) Add(p Product, qty int) error {
	if o.placed {
		return fail("order %d is placed, so it takes no new lines", o.Number)
	}
	if qty <= 0 {
		return fail("bad quantity %d", qty)
	}
	o.lines = append(o.lines, orderLine{p, qty})
	return nil
}

func (o *Order) Place() error {
	if len(o.lines) == 0 {
		return fail("order %d has no lines", o.Number)
	}
	o.placed = true
	return nil
}

func (o *Order) Subtotal() int {
	sum := 0
	for _, l := range o.lines {
		sum += l.qty * l.product.PriceCents
	}
	return sum
}

func (o *Order) Total() int {
	s := o.Subtotal()
	return s - o.Customer.Discount(s)
}

func main() {
	lamp, desk, pen := Product{"lamp", 2500}, Product{"desk", 7500}, Product{"pen", 500}
	orders := []*Order{
		{Number: 1, Customer: gold{person{"Ann"}}},
		{Number: 2, Customer: regular{person{"Bob"}}},
		{Number: 3, Customer: staff{person{"Cy"}}},
	}
	_ = orders[0].Add(lamp, 2)
	_ = orders[0].Add(desk, 1)
	for _, o := range orders[1:] {
		_ = o.Add(lamp, 1)
		_ = o.Add(pen, 3)
	}
	for _, o := range orders {
		_ = o.Place()
		fmt.Printf("order %d for %s (%s): subtotal %s, total %s\n",
			o.Number, o.Customer.Name(), o.Customer.Kind(), cents(o.Subtotal()), cents(o.Total()))
	}
	actions := []func() error{
		func() error { return orders[0].Add(pen, 1) },
		func() error { return (&Order{Number: 4, Customer: regular{person{"Di"}}}).Place() },
		func() error { return (&Order{Number: 5, Customer: regular{person{"Ed"}}}).Add(pen, 0) },
	}
	for _, action := range actions {
		var de *domainError
		if err := action(); errors.As(err, &de) {
			fmt.Println("error:", de)
		}
	}
}
```

```output
order 1 for Ann (gold): subtotal 125.00, total 112.50
order 2 for Bob (regular): subtotal 40.00, total 40.00
order 3 for Cy (staff): subtotal 40.00, total 32.00
error: order 1 is placed, so it takes no new lines
error: order 4 has no lines
error: bad quantity 0
```

- Verdict: idiom. Go has no inheritance, so `Customer` is an interface, and each kind of customer is a type that embeds `person` for the name. The fields `lines` and `placed` start with a lower-case letter, so code in other packages can change them only through the methods.
