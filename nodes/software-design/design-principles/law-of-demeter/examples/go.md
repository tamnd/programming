```go run id=paying-customer
package main

import (
	"errors"
	"fmt"
)

type wallet struct{ cash int }

func (w *wallet) take(amount int) error {
	if amount > w.cash {
		return errors.New("not enough cash")
	}
	w.cash -= amount
	return nil
}

type Customer struct{ w wallet }

func NewCustomer(cash int) *Customer { return &Customer{w: wallet{cash}} }

// Pay is the only way to take money; the wallet stays inside.
func (c *Customer) Pay(amount int) error { return c.w.take(amount) }

func (c *Customer) Balance() int { return c.w.cash }

func charge(c *Customer, amount int) error { return c.Pay(amount) }

func main() {
	c := NewCustomer(20)
	if err := charge(c, 7); err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println(c.Balance())
}
```

```output
13
```
