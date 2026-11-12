```go run id=query-and-modifier
package main

import (
	"fmt"
	"reflect"
)

type Customer struct {
	Name     string
	Invoices []int
}

// totalOutstanding is a query: it reads the customer and changes nothing.
func totalOutstanding(c Customer) int {
	total := 0
	for _, amount := range c.Invoices {
		total += amount
	}
	return total
}

// sendBill is a modifier: it adds a bill to the outbox and returns nothing.
func sendBill(c Customer, outbox *[]string) {
	*outbox = append(*outbox, fmt.Sprintf("bill to %s: %d", c.Name, totalOutstanding(c)))
}

func check(ok bool, what string) {
	if !ok {
		panic(what)
	}
}

func main() {
	var outbox []string
	ana := Customer{Name: "Ana", Invoices: []int{40, 2}}

	check(totalOutstanding(ana) == 42, "query")
	check(totalOutstanding(ana) == 42, "query again")
	check(len(outbox) == 0, "query sent nothing")

	total := totalOutstanding(ana)
	sendBill(ana, &outbox)
	check(total == 42, "total")
	check(reflect.DeepEqual(outbox, []string{"bill to Ana: 42"}), "outbox")
	fmt.Println("separate query from modifier: tests pass")
}
```

```output
separate query from modifier: tests pass
```
