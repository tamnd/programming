```go run id=order-warehouse-mapper
package main

import (
	"fmt"
	"strings"
)

// Subsystem 1: the order system. It knows nothing about the warehouse.

type orderLine struct {
	item string
	qty  int
}

type Order struct {
	Number   int
	Customer string
	Lines    []orderLine
	Status   string
}

// Subsystem 2: the warehouse. It knows nothing about orders. It works with
// product codes and reference strings, and it cannot be changed.

type pickLine struct {
	code string
	qty  int
}

type pickRequest struct {
	ref   string
	lines []pickLine
}

type pickReply struct{ ref, state string }

type Warehouse struct{}

var codes = map[string]bool{"PEN-01": true, "INK-02": true}

func (Warehouse) Pick(r pickRequest) pickReply {
	known := 0
	for _, l := range r.lines {
		if codes[l.code] {
			known++
		}
	}
	state := "SHORT"
	if known == len(r.lines) {
		state = "PICKED"
	}
	return pickReply{r.ref, state}
}

// OrderWarehouseMapper knows both subsystems and connects them.
// Neither of them knows the mapper.
type OrderWarehouseMapper struct{ warehouse Warehouse }

var codeOf = map[string]string{"pen": "PEN-01", "ink": "INK-02"}
var statusOf = map[string]string{"PICKED": "picked", "SHORT": "waiting for stock"}

func (m OrderWarehouseMapper) Send(o *Order) {
	req := pickRequest{ref: fmt.Sprintf("ORD-%d", o.Number)}
	var parts []string
	for _, l := range o.Lines {
		req.lines = append(req.lines, pickLine{codeOf[l.item], l.qty})
		parts = append(parts, fmt.Sprintf("%s x%d", codeOf[l.item], l.qty))
	}
	fmt.Printf("warehouse gets: ref %s, lines %s\n", req.ref, strings.Join(parts, ", "))
	reply := m.warehouse.Pick(req)
	fmt.Printf("warehouse replies: ref %s, state %s\n", reply.ref, reply.state)
	o.Status = statusOf[reply.state]
}

func main() {
	order := &Order{7, "ann", []orderLine{{"pen", 2}, {"ink", 1}}, "new"}
	var parts []string
	for _, l := range order.Lines {
		parts = append(parts, fmt.Sprintf("%d x %s", l.qty, l.item))
	}
	fmt.Printf("order %d for %s: %s, status %s\n", order.Number, order.Customer, strings.Join(parts, ", "), order.Status)
	OrderWarehouseMapper{Warehouse{}}.Send(order)
	fmt.Printf("order %d status: %s\n", order.Number, order.Status)
}
```

```output
order 7 for ann: 2 x pen, 1 x ink, status new
warehouse gets: ref ORD-7, lines PEN-01 x2, INK-02 x1
warehouse replies: ref ORD-7, state PICKED
order 7 status: picked
```

- Verdict: idiom. In a real program, the order system and the warehouse are two packages, and the mapper is a third package that imports both. Neither of the two imports the mapper.
- `Send` takes a pointer to the order, so that it can set the status.
