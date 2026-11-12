```go run id=let-the-code-say-it
package main

import (
	"errors"
	"fmt"
)

type Parcel struct{ Grams int }

// Before: comments explain what each block does.
func shippingCentsBefore(parcels []Parcel, zone int) int {
	// weight must be positive for every parcel
	// compute the total weight in grams
	grams := 0
	for _, p := range parcels {
		grams += p.Grams
	}
	// round up to whole kilograms
	kg := (grams + 999) / 1000
	// look up the rate for the zone (1 = domestic, 2 = europe, other = world)
	rate := 1600
	if zone == 1 {
		rate = 450
	} else if zone == 2 {
		rate = 900
	}
	// the carrier charges at least 2 kg per shipment, see contract section 4
	return max(kg, 2) * rate
}

// After: names and a check say what; one comment says why.

// Zone is a shipping zone. Its value is the rate in cents per kilogram.
type Zone int

const (
	Domestic Zone = 450
	Europe   Zone = 900
	World    Zone = 1600
)

func zoneOf(code int) Zone {
	switch code {
	case 1:
		return Domestic
	case 2:
		return Europe
	}
	return World
}

var ErrWeight = errors.New("every parcel must have a positive weight")

// BillableKilograms returns the weight to charge for, in whole kilograms,
// with the carrier's minimum applied. It returns ErrWeight if a parcel has no weight.
func BillableKilograms(parcels []Parcel) (int, error) {
	grams := 0
	for _, p := range parcels {
		if p.Grams <= 0 {
			return 0, ErrWeight
		}
		grams += p.Grams
	}
	kg := (grams + 999) / 1000
	// The carrier charges at least 2 kg per shipment (contract, section 4).
	return max(kg, 2), nil
}

// ShippingCents returns the price of a shipment of parcels to a zone.
func ShippingCents(parcels []Parcel, zone Zone) (int, error) {
	kg, err := BillableKilograms(parcels)
	return kg * int(zone), err
}

func must(n int, err error) int {
	if err != nil {
		panic(err)
	}
	return n
}

func main() {
	parcels := []Parcel{{700}, {1450}}
	for _, code := range []int{1, 2, 3} {
		if must(ShippingCents(parcels, zoneOf(code))) != shippingCentsBefore(parcels, code) {
			panic(fmt.Sprint("zone ", code))
		}
	}
	if must(ShippingCents([]Parcel{{300}}, Domestic)) != 900 || must(ShippingCents(parcels, Europe)) != 2700 {
		panic("prices")
	}
	if _, err := ShippingCents([]Parcel{{0}}, Domestic); !errors.Is(err, ErrWeight) {
		panic("the check must fail")
	}
	fmt.Println("shipping: tests pass")
}
```

```output
shipping: tests pass
```

- `BillableKilograms` and `ShippingCents` are exported, so each has a doc comment that starts with its name, as the Go guide asks. `go doc` prints these comments.
- Go has no assertions. The check returns an error, and the caller decides what to do with it.
