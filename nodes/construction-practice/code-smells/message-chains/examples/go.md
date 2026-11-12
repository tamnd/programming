```go run id=hide-the-path
package main

import "fmt"

type Address struct{ Country string }

type Customer struct{ Address Address }

type Order struct {
	Customer Customer
	Delivery *Address
}

// After: the order answers the question, and knows the path.
func (o Order) ShipsTo() string {
	if o.Delivery != nil {
		return o.Delivery.Country
	}
	return o.Customer.Address.Country
}

// Before: the client walks the chain.
func carrierBefore(o Order) string {
	if o.Customer.Address.Country == "US" {
		return "ups"
	}
	return "dhl"
}

// After: the client asks the order.
func carrier(o Order) string {
	if o.ShipsTo() == "US" {
		return "ups"
	}
	return "dhl"
}

func main() {
	home := Order{Customer: Customer{Address{"DE"}}}
	if carrierBefore(home) != carrier(home) {
		panic("home")
	}
	gift := Order{Customer: Customer{Address{"DE"}}, Delivery: &Address{"US"}}
	if carrierBefore(gift) != "dhl" { // reads the customer's address
		panic("before")
	}
	if carrier(gift) != "ups" {
		panic("after")
	}
	fmt.Println("ShipsTo: tests pass")
}
```

```output
ShipsTo: tests pass
```

- In Go, a chain of exported fields is as common as a chain of getters. `o.Customer.Address.Country` couples the client to two struct layouts.
- `Delivery` is a pointer, so `nil` means that the order has no delivery address of its own.
