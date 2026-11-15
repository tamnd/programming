```go run id=aggregate-parts
package main

import (
	"fmt"
	"sort"
	"strings"
)

// Msg is a message from a splitter.
type Msg struct {
	CID       string
	Seq, Size int
	Item      string
}

func main() {
	incoming := []Msg{
		{"A", 2, 3, "gadget"},
		{"B", 1, 2, "widget"},
		{"A", 1, 3, "widget"},
		{"B", 2, 2, "gizmo"},
		{"A", 2, 3, "gadget"}, // a second copy, as at-least-once delivery can give
		{"A", 3, 3, "widget"},
		{"B", 1, 2, "widget"}, // a copy that comes after B is complete
	}
	groups := map[string]map[int]string{}
	closed := map[string]bool{}

	for _, m := range incoming {
		if closed[m.CID] {
			fmt.Printf("late: %s %d/%d discarded, the group is closed\n", m.CID, m.Seq, m.Size)
			continue
		}
		parts, ok := groups[m.CID]
		if !ok {
			parts = map[int]string{}
			groups[m.CID] = parts
		}
		if _, dup := parts[m.Seq]; dup {
			fmt.Printf("duplicate: %s %d/%d ignored\n", m.CID, m.Seq, m.Size)
			continue
		}
		parts[m.Seq] = m.Item
		if len(parts) == m.Size { // the completion condition: all parts are here
			seqs := make([]int, 0, len(parts))
			for s := range parts {
				seqs = append(seqs, s)
			}
			sort.Ints(seqs)
			items := make([]string, 0, len(seqs))
			for _, s := range seqs {
				items = append(items, parts[s])
			}
			fmt.Printf("%s complete: %s\n", m.CID, strings.Join(items, ", "))
			delete(groups, m.CID)
			closed[m.CID] = true
		}
	}
	fmt.Printf("still open: %d\n", len(groups))
}
```

```output
B complete: widget, gizmo
duplicate: A 2/3 ignored
A complete: widget, gadget, widget
late: B 1/2 discarded, the group is closed
still open: 0
```

- Verdict: idiom. A `map[string]map[int]string` is the store of groups, and the form `_, dup := parts[m.Seq]` finds a second copy of a part.
- The sequence numbers are sorted with `sort.Ints` before the join, because the order of a Go map is not defined.
- A real aggregator reads from a channel in a loop with `select`, so that a timer can also close a group that does not complete.
