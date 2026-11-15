```go run id=route-orders
package main

import (
	"encoding/json"
	"fmt"
	"strings"
)

type order struct {
	ID   string `json:"id"`
	Type string `json:"type"`
	Qty  int    `json:"qty"`
}

// The router reads one input channel and writes to the channel of the system for each item type.
var routes = map[string]string{"widget": "inventory.widgets", "gadget": "inventory.gadgets"}
var names = []string{"inventory.widgets", "inventory.gadgets", "invalid"}
var channels = map[string][]string{}

func route(raw []byte) {
	var o order
	if err := json.Unmarshal(raw, &o); err != nil {
		channels["invalid"] = append(channels["invalid"], string(raw))
		return
	}
	channel, ok := routes[o.Type]
	if !ok {
		channel = "invalid" // an unknown type is not lost
	}
	channels[channel] = append(channels[channel], o.ID)
}

func main() {
	incoming := []order{{"o1", "widget", 3}, {"o2", "gadget", 1}, {"o3", "gizmo", 2}, {"o4", "widget", 1}}
	for _, o := range incoming {
		raw, err := json.Marshal(o)
		if err != nil {
			panic(err)
		}
		route(raw)
	}
	for _, name := range names {
		fmt.Printf("%s: %s\n", name, strings.Join(channels[name], " "))
	}
}
```

```output
inventory.widgets: o1 o4
inventory.gadgets: o2
invalid: o3
```

- Verdict: idiom. A `map[string]string` from the item type to the channel is the rule table, and the form `channel, ok := routes[o.Type]` finds the case with no rule.
- A message that is not valid JSON also goes to the invalid channel, with its raw bytes, so a person can read it.
- The slice `names` gives a fixed order for the output, because the order of a Go map is not defined.
