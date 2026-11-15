```go run id=bridge-queues
package main

import (
	"fmt"
	"strings"
)

const limit = 64 * 1024

var routes = map[string]string{"orders": "sb-orders", "invoices": "sb-invoices"}

type SourceMsg struct {
	ID, Channel, Label string
	Size               int
}

type DestMsg struct {
	ID, Subject string
	Size        int
}

func main() {
	source := []SourceMsg{
		{"m1", "orders", "new-order", 120}, {"m2", "invoices", "invoice", 300},
		{"m3", "orders", "new-order", 90}, {"m4", "orders", "bulk-order", 70000},
		{"m5", "invoices", "invoice", 210},
	}
	dest := map[string][]DestMsg{}
	var errors []string
	forwarded := map[string]bool{}
	crashed := false

	for len(source) > 0 {
		m := source[0] // peek: the message stays on the source until the ack
		switch {
		case m.Size > limit:
			fmt.Printf("%s too large for the destination (%d > %d bytes): to the bridge error queue\n", m.ID, m.Size, limit)
			errors = append(errors, m.ID)
		case forwarded[m.ID]:
			fmt.Printf("%s again: already forwarded, ack only\n", m.ID)
		default:
			target := routes[m.Channel]
			dest[target] = append(dest[target], DestMsg{m.ID, m.Label, m.Size})
			forwarded[m.ID] = true
			fmt.Printf("%s %s -> %s, %d bytes, subject %s\n", m.ID, m.Channel, target, m.Size, m.Label)
			if m.ID == "m3" && !crashed {
				crashed = true
				fmt.Println("the bridge stops before the ack of m3; the source delivers m3 again")
				continue
			}
		}
		source = source[1:] // the ack
	}

	var parts []string
	for _, name := range []string{"sb-orders", "sb-invoices"} {
		var ids []string
		for _, m := range dest[name] {
			ids = append(ids, m.ID)
		}
		parts = append(parts, name+": "+strings.Join(ids, " "))
	}
	parts = append(parts, "bridge errors: "+strings.Join(errors, " "))
	fmt.Println(strings.Join(parts, " | "))
}
```

```output
m1 orders -> sb-orders, 120 bytes, subject new-order
m2 invoices -> sb-invoices, 300 bytes, subject invoice
m3 orders -> sb-orders, 90 bytes, subject new-order
the bridge stops before the ack of m3; the source delivers m3 again
m3 again: already forwarded, ack only
m4 too large for the destination (70000 > 65536 bytes): to the bridge error queue
m5 invoices -> sb-invoices, 210 bytes, subject invoice
sb-orders: m1 m3 | sb-invoices: m2 m5 | bridge errors: m4
```

- Verdict: idiom. A slice is the source queue; `source[0]` is the peek and `source[1:]` is the ack.
- A `switch` with no tag checks the size limit, then the de-dup map, then forwards.
- The program prints the destinations in a fixed order, because the order of a `range` over a map is not fixed in Go.
