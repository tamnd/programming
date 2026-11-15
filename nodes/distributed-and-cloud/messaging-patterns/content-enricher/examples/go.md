```go run id=enrich-orders
package main

import (
	"fmt"
	"time"
)

type Order struct {
	ID, Customer, Zip string
}

type Enriched struct {
	Order
	Name, State string
	Received    time.Time
}

var zipPrefix = map[string]string{"981": "WA", "100": "NY", "606": "IL"}
var directory = map[string]string{"c1": "Ada Park", "c2": "Lin Tran"}

type Enricher struct {
	now     func() time.Time
	lookups int
}

func (e *Enricher) Enrich(o Order) (Enriched, error) {
	e.lookups++
	name, ok := directory[o.Customer]
	if !ok {
		return Enriched{}, fmt.Errorf("customer %s not found", o.Customer)
	}
	state, ok := zipPrefix[o.Zip[:3]]
	if !ok {
		state = "??"
	}
	return Enriched{Order: o, Name: name, State: state, Received: e.now()}, nil
}

func main() {
	t := time.Date(2026, 1, 5, 9, 0, 0, 0, time.UTC)
	clock := func() time.Time { t = t.Add(time.Second); return t }
	e := &Enricher{now: clock}
	incoming := []Order{{"o1", "c1", "98101"}, {"o2", "c2", "10001"}, {"o3", "c1", "60601"}, {"o4", "c9", "98101"}}
	enriched, invalid := 0, 0
	for _, o := range incoming {
		m, err := e.Enrich(o)
		if err != nil {
			invalid++
			fmt.Printf("invalid: %s %v\n", o.ID, err)
			continue
		}
		enriched++
		fmt.Printf("%s %s %s, %s %s, received %s\n", m.ID, m.Customer, m.Name, m.Zip, m.State, m.Received.Format("15:04:05"))
	}
	fmt.Printf("lookups: %d, enriched: %d, invalid: %d\n", e.lookups, enriched, invalid)
}
```

```output
o1 c1 Ada Park, 98101 WA, received 09:00:01
o2 c2 Lin Tran, 10001 NY, received 09:00:02
o3 c1 Ada Park, 60601 IL, received 09:00:03
invalid: o4 customer c9 not found
lookups: 4, enriched: 3, invalid: 1
```

- Verdict: idiom. The `Enriched` struct embeds `Order`, so the original fields stay, and the new fields are next to them.
- The enricher gets the clock as a `func() time.Time`, so the program can give it a fixed start.
- A missing customer gives an `error`, and the comma-ok form of the map lookup tells a miss from an empty value.
