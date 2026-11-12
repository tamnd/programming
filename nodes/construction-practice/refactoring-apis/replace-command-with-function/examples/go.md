```go run id=command-to-function
package main

import "fmt"

type Customer struct{ Rate float64 }

type chargeCalculator struct {
	customer Customer
	usage    float64
}

func (c chargeCalculator) execute() float64 { return c.customer.Rate * c.usage }

func charge(customer Customer, usage float64) float64 { return customer.Rate * usage }

func main() {
	customer := Customer{Rate: 0.25}
	if charge(customer, 40) != (chargeCalculator{customer, 40}).execute() {
		panic("mismatch")
	}
	// A caller that wants a value of a function type can pass charge itself.
	var f func(Customer, float64) float64 = charge
	fmt.Println(f(customer, 40))
}
```

```output
10
```
