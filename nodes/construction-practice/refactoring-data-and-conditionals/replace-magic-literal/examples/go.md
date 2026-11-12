```go run id=name-the-number
package main

import "fmt"

// Cents is its own type, so a weight in grams cannot be added to it by mistake.
type Cents int

const (
	baseFee             Cents = 450
	feePerExtraKilogram Cents = 120
	freeWeightGrams           = 2000
	gramsPerKilogram          = 1000
	taxPercent                = 25
)

func parcelCostBefore(grams int) int {
	cents := 450
	if grams > 2000 {
		cents += (grams - 2000 + 999) / 1000 * 120
	}
	return (cents*125 + 50) / 100
}

func parcelCost(grams int) Cents {
	cents := baseFee
	if grams > freeWeightGrams {
		extraKilograms := (grams - freeWeightGrams + gramsPerKilogram - 1) / gramsPerKilogram
		cents += Cents(extraKilograms) * feePerExtraKilogram
	}
	return (cents*(100+taxPercent) + 50) / 100
}

func main() {
	for _, g := range []int{0, 2000, 2001, 3001, 10000} {
		if int(parcelCost(g)) != parcelCostBefore(g) {
			panic(fmt.Sprint("differs at ", g))
		}
	}
	// cents += grams would not compile: mismatched types Cents and int.
	fmt.Println("replace magic literal: tests pass")
}
```

```output
replace magic literal: tests pass
```
