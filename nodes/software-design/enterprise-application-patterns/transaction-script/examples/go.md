```go run id=transaction-script
package main

import "fmt"

type customer struct {
	name   string
	credit int
}

type order struct {
	customer, cents int
	state           string
}

// A thin wrapper over the stored tables. The scripts call it directly.
var (
	customers = map[int]*customer{1: {name: "ada", credit: 5000}}
	orders    = map[int]*order{}
)

func findCustomer(customerID int) *customer { return customers[customerID] }

func insertOrder(customerID, cents int) int {
	orderID := len(orders) + 1
	orders[orderID] = &order{customer: customerID, cents: cents, state: "open"}
	return orderID
}

func money(cents int) string { return fmt.Sprintf("%d.%02d", cents/100, cents%100) }

// One transaction script for each request from the presentation.
func placeOrder(customerID, cents int) string {
	c := findCustomer(customerID)
	if c.credit < cents {
		return "refused, credit left " + money(c.credit)
	}
	c.credit -= cents
	return fmt.Sprintf("order %d", insertOrder(customerID, cents))
}

func cancelOrder(orderID int) string {
	o := orders[orderID]
	if o.state != "open" {
		return "already cancelled"
	}
	o.state = "cancelled"
	findCustomer(o.customer).credit += o.cents
	return "refunded " + money(o.cents)
}

func main() {
	fmt.Println("place_order(ada, 30.00):", placeOrder(1, 3000))
	fmt.Println("place_order(ada, 30.00):", placeOrder(1, 3000))
	fmt.Println("cancel_order(1):", cancelOrder(1))
	fmt.Println("cancel_order(1):", cancelOrder(1))
	fmt.Println("place_order(ada, 30.00):", placeOrder(1, 3000))
	fmt.Println("credit of ada:", money(findCustomer(1).credit))
}
```

```output
place_order(ada, 30.00): order 1
place_order(ada, 30.00): refused, credit left 20.00
cancel_order(1): refunded 30.00
cancel_order(1): already cancelled
place_order(ada, 30.00): order 2
credit of ada: 20.00
```

- Verdict: idiom. Each script is a package-level function, and the maps hold pointers to the rows, so a script can change a row in place.
