```go run id=compute-once
package main

import (
	"fmt"
	"math"
	"slices"
)

const divisor = 5000.0 // cm^3 per kg of volumetric weight

type Parcel struct {
	ID                    string
	Length, Width, Height float64
	WeightKg              float64
	Tags                  []string
}

// The output type says that the derived fields are there.
type EnrichedParcel struct {
	Parcel
	Volume       float64
	ChargeableKg float64
}

func enrichParcel(p Parcel) EnrichedParcel {
	p.Tags = slices.Clone(p.Tags) // a struct copy shares the slice; clone it
	v := p.Length * p.Width * p.Height
	return EnrichedParcel{Parcel: p, Volume: v, ChargeableKg: math.Ceil(math.Max(p.WeightKg, v/divisor))}
}

func main() {
	raw := Parcel{ID: "P1", Length: 40, Width: 30, Height: 20, WeightKg: 2, Tags: []string{"fragile"}}
	e := enrichParcel(raw)
	e.Tags[0] = "express"
	fmt.Println(e.ID, e.ChargeableKg, raw.Tags)
	if e.ChargeableKg != 5 || raw.Tags[0] != "fragile" {
		panic("unexpected result")
	}
	fmt.Println("combine functions into transform: tests pass")
}
```

```output
P1 5 [fragile]
combine functions into transform: tests pass
```
