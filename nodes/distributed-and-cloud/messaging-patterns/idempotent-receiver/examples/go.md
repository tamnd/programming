```go run id=payment-receiver
package main

import "fmt"

const window = 3

type delivery struct {
	id           string
	order, cents int
}

type receiver struct {
	order  []string // ids in the window, oldest first
	seen   map[string]bool
	totals map[int]int
	ever   map[string]bool // only to explain the output
}

func (r *receiver) receive(d delivery) string {
	if r.seen[d.id] {
		return d.id + ": duplicate, ignored"
	}
	r.totals[d.order] += d.cents
	r.seen[d.id] = true
	r.order = append(r.order, d.id)
	if len(r.order) > window {
		delete(r.seen, r.order[0])
		r.order = r.order[1:]
	}
	note := ""
	if r.ever[d.id] {
		note = fmt.Sprintf(" (applied twice: the id left the window of %d ids)", window)
	}
	r.ever[d.id] = true
	return fmt.Sprintf("%s: order %d +%d, total %d%s", d.id, d.order, d.cents, r.totals[d.order], note)
}

func main() {
	deliveries := []delivery{
		{"pay-1", 17, 1250}, {"pay-2", 18, 800}, {"pay-1", 17, 1250},
		{"pay-3", 17, 300}, {"pay-4", 19, 500}, {"pay-5", 20, 100},
		{"pay-2", 18, 800},
	}
	r := &receiver{seen: map[string]bool{}, totals: map[int]int{}, ever: map[string]bool{}}
	for _, d := range deliveries {
		fmt.Println(r.receive(d))
	}
}
```

```output
pay-1: order 17 +1250, total 1250
pay-2: order 18 +800, total 800
pay-1: duplicate, ignored
pay-3: order 17 +300, total 1550
pay-4: order 19 +500, total 500
pay-5: order 20 +100, total 100
pay-2: order 18 +800, total 1600 (applied twice: the id left the window of 3 ids)
```

- Verdict: idiom. A map gives the lookup of an id, and a slice keeps the ids in the order of arrival, so the oldest one can be removed.
- `r.totals[d.order] += d.cents` works for a new order too, because a missing key reads as 0.
- The map `ever` exists only to explain the output; a real receiver has no such map.
