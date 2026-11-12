```go run id=encapsulate-global
package main

import (
	"fmt"
	"math"
)

// Before: an exported package variable, so any package can write it.
var TaxRate = 0.2

func priceBefore(cents int) int { return int(math.Round(float64(cents) * (1 + TaxRate))) }

func exportOrderBefore(cents int) int {
	TaxRate = 0               // the export is tax free
	return priceBefore(cents) // and nothing puts the rate back
}

// After: an unexported variable, with accessor functions.
var taxRate = 0.2

func Rate() float64 { return taxRate }

func SetRate(r float64) error {
	if !(r >= 0 && r <= 1) {
		return fmt.Errorf("tax rate %v", r)
	}
	taxRate = r
	return nil
}

func priceWithRate(cents int, rate float64) int { return int(math.Round(float64(cents) * (1 + rate))) }
func price(cents int) int                       { return priceWithRate(cents, Rate()) }
func exportOrder(cents int) int                 { return priceWithRate(cents, 0) }

func main() {
	if priceBefore(1000) != 1200 || exportOrderBefore(1000) != 1000 || priceBefore(1000) != 1000 {
		panic("before")
	}
	fmt.Println("before: the export changed the domestic price")

	if price(1000) != 1200 || exportOrder(1000) != 1000 || price(1000) != 1200 || SetRate(20) == nil {
		panic("after")
	}
	fmt.Println("after: tests pass")
}
```

```output
before: the export changed the domestic price
after: tests pass
```

- In Go, a package-level name that starts with a capital letter is exported. `TaxRate` can be written by any package that imports this one. `taxRate` can be written only by code in the same package, and other packages must call `SetRate`.
- `SetRate` returns an `error` in place of a panic, which is the usual Go way to report a bad argument that the caller can handle.
- The linter `gochecknoglobals` reports both versions, because each has a package-level `var`. Encapsulation is the first step. Passing the rate to `priceWithRate` is the step that removes the hidden input.
