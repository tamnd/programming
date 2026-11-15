```go run id=order-events
package main

import (
	"encoding/json"
	"fmt"
)

// cloudEvent has the required CloudEvents attributes and the data of the event.
type cloudEvent struct {
	SpecVersion string         `json:"specversion"`
	ID          string         `json:"id"`
	Source      string         `json:"source"`
	Type        string         `json:"type"`
	Data        map[string]int `json:"data"`
}

type key struct{ source, id string }

type subscriber struct {
	name     string
	handlers map[string]func(map[string]int) string
	seen     map[key]bool
}

func (s *subscriber) deliver(raw []byte) {
	var e cloudEvent
	if err := json.Unmarshal(raw, &e); err != nil {
		fmt.Printf("%s: invalid message: %v\n", s.name, err)
		return
	}
	k := key{e.Source, e.ID}
	if s.seen[k] {
		fmt.Printf("%s: skip duplicate %s\n", s.name, e.ID)
		return
	}
	s.seen[k] = true
	h, ok := s.handlers[e.Type]
	if !ok {
		fmt.Printf("%s: ignore %s\n", s.name, e.Type)
		return
	}
	fmt.Printf("%s: %s\n", s.name, h(e.Data))
}

func event(id, typ string, data map[string]int) []byte {
	raw, err := json.Marshal(cloudEvent{"1.0", id, "/shop", "com.example.order." + typ, data})
	if err != nil {
		panic(err)
	}
	return raw
}

func main() {
	billing := &subscriber{"billing", map[string]func(map[string]int) string{
		"com.example.order.placed": func(d map[string]int) string {
			return fmt.Sprintf("charge order %d, %d cents", d["order"], d["cents"])
		},
		"com.example.order.cancelled": func(d map[string]int) string {
			return fmt.Sprintf("refund order %d", d["order"])
		},
	}, map[key]bool{}}
	email := &subscriber{"email", map[string]func(map[string]int) string{
		"com.example.order.placed": func(d map[string]int) string {
			return fmt.Sprintf("mail the confirmation of order %d", d["order"])
		},
	}, map[key]bool{}}
	topic := []*subscriber{billing, email} // each subscriber gets a copy of each event

	events := [][]byte{
		event("e1", "placed", map[string]int{"order": 41, "cents": 1999}),
		event("e2", "placed", map[string]int{"order": 42, "cents": 500}),
		event("e2", "placed", map[string]int{"order": 42, "cents": 500}), // sent again after a network error
		event("e3", "cancelled", map[string]int{"order": 41}),
	}
	for _, raw := range events {
		for _, s := range topic {
			s.deliver(raw)
		}
	}
}
```

```output
billing: charge order 41, 1999 cents
email: mail the confirmation of order 41
billing: charge order 42, 500 cents
email: mail the confirmation of order 42
billing: skip duplicate e2
email: skip duplicate e2
billing: refund order 41
email: ignore com.example.order.cancelled
```

- Verdict: idiom. Go has no publish-subscribe channel in the standard library. A slice of subscribers is the topic, and `encoding/json` encodes the CloudEvents attributes from the struct tags.
- The struct `key` with the source and the id is the key of the `seen` map, because Go can compare structs of strings.
- A Go channel delivers each value to one receiver only. A topic with channels needs one channel for each subscriber and a loop that copies each event to all of them.
