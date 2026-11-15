```go run id=one-script-per-request
package main

import (
	"errors"
	"fmt"
)

type customer struct{ name, tier string }
type product struct {
	name       string
	priceCents int
}
type orderLine struct{ orderID, productID, qty int }

// The tables of the database, kept in memory for this example.
var (
	customers   = map[int]customer{1: {"Ann", "gold"}, 2: {"Bob", "regular"}}
	products    = map[int]product{1: {"lamp", 2500}, 2: {"desk", 7500}, 3: {"pen", 500}}
	orders      = map[int]int{} // order id to customer id
	orderLines  []orderLine
	nextOrderID = 1
)

// scriptError is the error of a script that the user can correct.
type scriptError struct{ msg string }

func (e *scriptError) Error() string { return e.msg }

func fail(format string, args ...any) error { return &scriptError{fmt.Sprintf(format, args...)} }

func cents(n int) string { return fmt.Sprintf("%d.%02d", n/100, n%100) }

// Transaction script 1: place an order. Each item is {productID, qty}.
func placeOrder(customerID int, items [][2]int) (int, string, error) {
	c, ok := customers[customerID]
	if !ok {
		return 0, "", fail("no customer %d", customerID)
	}
	// Check everything first, so that a failed script writes nothing.
	for _, it := range items {
		if it[1] <= 0 {
			return 0, "", fail("bad quantity %d", it[1])
		}
		if _, ok := products[it[0]]; !ok {
			return 0, "", fail("no product %d", it[0])
		}
	}
	id := nextOrderID
	nextOrderID++
	orders[id] = customerID
	for _, it := range items {
		orderLines = append(orderLines, orderLine{id, it[0], it[1]})
	}
	return id, c.name, nil
}

// Transaction script 2: show the total of an order.
func orderTotal(orderID int) (subtotal, total int, err error) {
	customerID, ok := orders[orderID]
	if !ok {
		return 0, 0, fail("no order %d", orderID)
	}
	tier := customers[customerID].tier
	for _, l := range orderLines {
		if l.orderID == orderID {
			subtotal += l.qty * products[l.productID].priceCents
		}
	}
	total = subtotal
	if tier == "gold" && subtotal >= 10000 {
		total = subtotal - subtotal/10
	}
	return subtotal, total, nil
}

func main() {
	for _, r := range []struct {
		customerID int
		items      [][2]int
	}{{1, [][2]int{{1, 2}, {2, 1}}}, {2, [][2]int{{1, 1}, {3, 3}}}} {
		id, name, _ := placeOrder(r.customerID, r.items)
		fmt.Printf("placed order %d for %s\n", id, name)
	}
	for _, id := range []int{1, 2} {
		s, t, _ := orderTotal(id)
		fmt.Printf("order %d: subtotal %s, total %s\n", id, cents(s), cents(t))
	}
	for _, r := range []struct {
		customerID int
		items      [][2]int
	}{{1, [][2]int{{9, 1}}}, {2, [][2]int{{1, 0}}}} {
		_, _, err := placeOrder(r.customerID, r.items)
		var se *scriptError
		if errors.As(err, &se) {
			fmt.Println("error:", se)
		}
	}
	fmt.Println("orders in the database:", len(orders))
}
```

```output
placed order 1 for Ann
placed order 2 for Bob
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
error: no product 9
error: bad quantity 0
orders in the database: 2
```

- Verdict: idiom. Each script is a function that returns an error. `scriptError` marks the errors that the user can correct, and the caller finds them with `errors.As`. In a Go service, each script would take a `*sql.Tx` and run in one transaction.
