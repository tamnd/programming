```go run id=near-its-data
package main

import (
	"fmt"
	"math"
)

type Parcel struct {
	WeightKg float64
	CM       [3]float64
}

// ShippingCents moved here from Order: it uses only the parcel's data.
func (p Parcel) ShippingCents() int {
	volumetricKg := p.CM[0] * p.CM[1] * p.CM[2] / 5000
	return 400 + int(math.Ceil(math.Max(p.WeightKg, volumetricKg)))*150
}

type Order struct {
	ID     int
	Parcel Parcel
}

// ShippingCents delegates while the callers change.
func (o Order) ShippingCents() int { return o.Parcel.ShippingCents() }

func main() {
	light := Order{1, Parcel{0.8, [3]float64{10, 10, 5}}}
	bulky := Order{2, Parcel{1.2, [3]float64{60, 40, 40}}}
	if light.ShippingCents() != 550 || bulky.ShippingCents() != 3400 {
		panic(fmt.Sprint(light.ShippingCents(), bulky.ShippingCents()))
	}
	fmt.Println("move function: tests pass")
}
```

```output
move function: tests pass
```
