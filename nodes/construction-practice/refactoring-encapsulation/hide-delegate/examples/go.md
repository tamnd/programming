```go run id=ask-the-server
package main

import "fmt"

type Warehouse struct {
	Code     string
	TimeZone string
}

// The warehouse field is unexported, so code in other packages
// cannot reach through a Shipment to its Warehouse.
type Shipment struct {
	ID        int
	warehouse Warehouse
}

func NewShipment(id int, w Warehouse) Shipment {
	return Shipment{ID: id, warehouse: w}
}

// TimeZone forwards to the delegate.
func (s Shipment) TimeZone() string { return s.warehouse.TimeZone }

func main() {
	s := NewShipment(7, Warehouse{"AMS1", "Europe/Amsterdam"})
	// Before, a client wrote: s.Warehouse.TimeZone
	if got := s.TimeZone(); got != "Europe/Amsterdam" {
		panic(got)
	}
	fmt.Println("hide delegate: tests pass")
}
```

```output
hide delegate: tests pass
```
