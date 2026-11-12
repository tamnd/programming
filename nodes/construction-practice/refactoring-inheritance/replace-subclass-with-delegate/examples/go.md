```go run id=subclass-to-delegate
package main

import "fmt"

// shipper is the axis of variation. A delegate in a field replaces a subclass.
type shipper interface{ DaysToShip() int }

type Warehouse struct{ Days int }

func (w Warehouse) DaysToShip() int { return w.Days }

type PriorityPlan struct{ Days int }

func (p PriorityPlan) DaysToShip() int { return p.Days }

type Order struct{ shipping shipper }

func (o *Order) DaysToShip() int        { return o.shipping.DaysToShip() }
func (o *Order) MakePriority(p PriorityPlan) { o.shipping = p }

func main() {
	order := &Order{shipping: Warehouse{Days: 5}}
	days := []int{order.DaysToShip()}
	order.MakePriority(PriorityPlan{Days: 2})
	days = append(days, order.DaysToShip())
	fmt.Println(days)
}
```

```output
[5 2]
```
