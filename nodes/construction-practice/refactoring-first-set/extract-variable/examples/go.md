```go run id=name-the-parts
package main

import (
	"fmt"
	"math"
)

// Before: one expression, three ideas.
func priceBefore(quantity, itemPrice float64) float64 {
	return quantity*itemPrice -
		math.Max(0, quantity-500)*itemPrice*0.05 +
		math.Min(quantity*itemPrice*0.1, 100)
}

// After: each idea has a name. gopls names an extracted variable newVar;
// the rename to a name that says the role is the programmer's step.
func price(quantity, itemPrice float64) float64 {
	basePrice := quantity * itemPrice
	quantityDiscount := math.Max(0, quantity-500) * itemPrice * 0.05
	shipping := math.Min(basePrice*0.1, 100)
	return basePrice - quantityDiscount + shipping
}

func main() {
	for _, c := range [][2]float64{{1, 10}, {500, 2}, {501, 2}, {2000, 3}} {
		if math.Abs(price(c[0], c[1])-priceBefore(c[0], c[1])) > 1e-9 {
			panic(fmt.Sprint("price ", c))
		}
	}
	fmt.Println("price: tests pass")
}
```

```output
price: tests pass
```

Go allows `const` only for constant expressions, so the extracted names are variables declared with `:=`. Each one is assigned once, which keeps it a name for one value. The test compares the two results with a tolerance, as is usual for floating-point values.
