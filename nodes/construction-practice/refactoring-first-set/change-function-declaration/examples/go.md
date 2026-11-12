```go run id=rename-in-stages
package main

import (
	"fmt"
	"math"
)

type circle struct{ radius float64 }

// Before: a cryptic name, and a parameter wider than the need.
func circum(c circle) float64 {
	return 2 * math.Pi * c.radius
}

// The body moves to a function with the declaration we want.
func circumference(radius float64) float64 {
	return 2 * math.Pi * radius
}

// circumOld is the wrapper that callers use until they move.
//
// Deprecated: use circumference.
func circumOld(c circle) float64 {
	return circumference(c.radius)
}

// Go has no default values. To add a parameter, the old function
// keeps its declaration and passes the value for its callers.
func priceWithDiscount(quantity int, unitPrice, discount float64) float64 {
	return float64(quantity) * unitPrice * (1 - discount)
}

func price(quantity int, unitPrice float64) float64 {
	return priceWithDiscount(quantity, unitPrice, 0)
}

func main() {
	for _, c := range []circle{{0}, {1}, {2.5}} {
		if circumOld(c) != circum(c) || circumference(c.radius) != circum(c) {
			panic("behaviour changed")
		}
	}
	if price(3, 10) != 30 || priceWithDiscount(3, 10, 0.5) != 15 {
		panic("unexpected price")
	}
	fmt.Println("change function declaration: tests pass")
}
```

```output
change function declaration: tests pass
```
