```go run id=unknown-customer
package main

import "fmt"

// Customer is the interface that callers use.
type Customer interface {
	IsKnown() bool
	BillLine() string
}

type knownCustomer struct {
	id         int
	name, plan string
}

func (c knownCustomer) IsKnown() bool { return true }

func (c knownCustomer) BillLine() string {
	return fmt.Sprintf("customer %d: %s, plan %s", c.id, c.name, c.plan)
}

// unknownCustomer is the special case: the same interface, with the behavior for a customer with no record.
type unknownCustomer struct{ id int }

func (c unknownCustomer) IsKnown() bool { return false }

func (c unknownCustomer) BillLine() string {
	return fmt.Sprintf("customer %d: current resident, plan basic", c.id)
}

var rows = map[int][2]string{1: {"Ann", "premium"}, 2: {"Bob", "basic"}}

func findCustomer(id int) Customer {
	row, ok := rows[id]
	if !ok {
		return unknownCustomer{id} // not nil
	}
	return knownCustomer{id, row[0], row[1]}
}

func main() {
	var customers []Customer
	for _, id := range []int{1, 2, 9} {
		customers = append(customers, findCustomer(id))
	}
	known := 0
	for _, c := range customers {
		fmt.Println(c.BillLine()) // no test for nil
		if c.IsKnown() {
			known++
		}
	}
	fmt.Println("known customers:", known)
}
```

```output
customer 1: Ann, plan premium
customer 2: Bob, plan basic
customer 9: current resident, plan basic
known customers: 2
```

- Verdict: idiom. Go has no subclasses, so `Customer` is an interface and the special case is a second type that implements it.
- A nil interface value would panic on `c.BillLine()`. The finder returns `unknownCustomer` and never nil.
