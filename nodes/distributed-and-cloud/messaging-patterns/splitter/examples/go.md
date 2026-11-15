```go run id=split-order
package main

import "fmt"

// Line is one line of an order.
type Line struct {
	Kind string
	Qty  int
}

// Msg is one message from the splitter, with the headers that an aggregator needs.
type Msg struct {
	CorrelationID string
	Seq, Size     int
	Line          *Line
}

func split(orderID string, lines []Line) []Msg {
	if len(lines) == 0 {
		// One message with size 0, so that a later aggregator is not left waiting.
		return []Msg{{CorrelationID: orderID}}
	}
	out := make([]Msg, 0, len(lines))
	for i := range lines {
		out = append(out, Msg{CorrelationID: orderID, Seq: i + 1, Size: len(lines), Line: &lines[i]})
	}
	return out
}

func main() {
	ids := []string{"A7", "B9"}
	orders := map[string][]Line{
		"A7": {{"widget", 2}, {"gadget", 1}, {"widget", 5}},
		"B9": {},
	}
	channel := map[string]string{"widget": "inventory.widgets", "gadget": "inventory.gadgets"}
	for _, id := range ids {
		fmt.Printf("order %s has %d lines\n", id, len(orders[id]))
		for _, m := range split(id, orders[id]) {
			if m.Line == nil {
				fmt.Printf("  %s 0/0 empty order -> orders.empty\n", id)
				continue
			}
			fmt.Printf("  %s %d/%d %s x%d -> %s\n", id, m.Seq, m.Size, m.Line.Kind, m.Line.Qty, channel[m.Line.Kind])
		}
	}
}
```

```output
order A7 has 3 lines
  A7 1/3 widget x2 -> inventory.widgets
  A7 2/3 gadget x1 -> inventory.gadgets
  A7 3/3 widget x5 -> inventory.widgets
order B9 has 0 lines
  B9 0/0 empty order -> orders.empty
```

- Verdict: idiom. A loop over the slice of lines appends one `Msg` for each line; `make` with the capacity `len(lines)` sizes the result once.
- `Line` is a pointer in `Msg`, so the message for an empty order has a `nil` line and is easy to test.
- The slice `ids` gives a fixed order for the output, because the order of a Go map is not defined.
