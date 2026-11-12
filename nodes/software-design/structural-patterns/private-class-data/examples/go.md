```go run id=order-data
package main

import "fmt"

// orderData holds the fields that are fixed after construction. In a real
// program, Order and orderData live in their own package, so that code in
// other packages can read the fields only through the methods.
type orderData struct {
	orderID  int
	customer string
}

type Order struct {
	data  *orderData // fixed after construction
	items []string   // changes
}

func NewOrder(orderID int, customer string, items []string) *Order {
	return &Order{data: &orderData{orderID, customer}, items: append([]string(nil), items...)}
}

func (o *Order) OrderID() int       { return o.data.orderID }
func (o *Order) Customer() string   { return o.data.customer }
func (o *Order) Add(item string)    { o.items = append(o.items, item) }
func (o *Order) Describe() string {
	return fmt.Sprintf("order %d for %s: %d items", o.OrderID(), o.Customer(), len(o.items))
}

func main() {
	order := NewOrder(42, "ann", []string{"pen", "ink"})
	fmt.Println(order.Describe())
	order.Add("paper")
	fmt.Println(order.Describe())
}
```

```output
order 42 for ann: 2 items
order 42 for ann: 3 items
```

- Verdict: idiom. Go has no final fields, so privacy comes from the package: unexported fields cannot be written from another package. The `os` package uses this form for `File`.
- Inside the package, every method can still write the fields; the protection is only against other packages.
