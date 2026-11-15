```go run id=tap-audit
package main

import (
	"fmt"
	"maps"
	"strings"
)

type Message struct {
	ID, Card string
	Total    int
	Headers  map[string]string
}

// WireTap sends each message to the main channel and a copy to the tap channel.
type WireTap struct {
	main, tap chan Message
	enabled   bool
}

func (w *WireTap) Send(m Message) {
	if w.enabled {
		// The struct copy copies the strings and the int, but a map is shared unless it is cloned.
		c := m
		c.Headers = maps.Clone(m.Headers)
		w.tap <- c
	}
	w.main <- m
}

func main() {
	w := &WireTap{main: make(chan Message, 10), tap: make(chan Message, 10), enabled: true}
	orders := []Message{{"o1", "4111111111111111", 30, nil}, {"o2", "5500000000000004", 12, nil},
		{"o3", "4111111111111111", 7, nil}, {"o4", "340000000000009", 45, nil}}
	for _, o := range orders {
		o.Headers = map[string]string{"source": "web"}
		w.Send(o)
		if o.ID == "o3" {
			w.enabled = false // an operator turns the tap off
		}
	}
	close(w.main)
	close(w.tap)
	var seen []Message
	for m := range w.main {
		seen = append(seen, m)
		fmt.Printf("main: %s card %s total %d\n", m.ID, m.Card, m.Total)
	}
	audited := 0
	for m := range w.tap {
		m.Card = strings.Repeat("*", len(m.Card)-4) + m.Card[len(m.Card)-4:]
		m.Headers["audit"] = "yes"
		audited++
		fmt.Printf("audit: %s card %s total %d\n", m.ID, m.Card, m.Total)
	}
	fmt.Printf("main: %d messages, audit: %d messages (tap off after o3)\n", len(seen), audited)
	unchanged := true
	for _, m := range seen {
		if _, ok := m.Headers["audit"]; ok || strings.Contains(m.Card, "*") {
			unchanged = false
		}
	}
	fmt.Println("main messages unchanged:", unchanged)
}
```

```output
main: o1 card 4111111111111111 total 30
main: o2 card 5500000000000004 total 12
main: o3 card 4111111111111111 total 7
main: o4 card 340000000000009 total 45
audit: o1 card ************1111 total 30
audit: o2 card ************0004 total 12
audit: o3 card ************1111 total 7
main: 4 messages, audit: 3 messages (tap off after o3)
main messages unchanged: true
```

- Verdict: idiom. Two buffered channels are the main and the tap channel, and `Send` writes to both.
- An assignment of a struct copies its strings, but a map field still points to the same map, so `maps.Clone` copies the headers.
- `close` and `range` drain each channel after the sends.
