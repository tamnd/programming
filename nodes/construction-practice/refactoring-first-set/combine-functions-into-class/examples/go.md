```go run id=one-home
package main

import (
	"fmt"
	"math"
)

const divisor = 5000.0 // cm^3 per kg of volumetric weight

type parcelRecord struct{ length, width, height, weightKg float64 }

// Before: functions that each take the record.
func chargeableWeight(p parcelRecord) float64 {
	return math.Max(p.weightKg, p.length*p.width*p.height/divisor)
}

func shippingCost(p parcelRecord, ratePerKg float64) float64 {
	return math.Ceil(chargeableWeight(p)) * ratePerKg
}

// After: in Go the class is a type with methods.
type Parcel struct{ length, width, height, weightKg float64 }

func (p Parcel) Volume() float64 { return p.length * p.width * p.height }

func (p Parcel) ChargeableWeight() float64 { return math.Max(p.weightKg, p.Volume()/divisor) }

func (p Parcel) ShippingCost(ratePerKg float64) float64 {
	return math.Ceil(p.ChargeableWeight()) * ratePerKg
}

func main() {
	records := []parcelRecord{{40, 30, 20, 2}, {10, 10, 10, 7}}
	for _, r := range records {
		p := Parcel(r) // the same fields, so a conversion works
		if p.ShippingCost(3) != shippingCost(r, 3) {
			panic("behaviour changed")
		}
		fmt.Println(p.ShippingCost(3))
	}
	fmt.Println("combine functions into class: tests pass")
}
```

```output
15
21
combine functions into class: tests pass
```
