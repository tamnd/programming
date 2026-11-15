```go run id=add-a-program
package main

import (
	"fmt"
	"strconv"
	"strings"
)

// message is the canonical message of the bus.
type message struct {
	Type        string
	Order       int
	AmountCents int
}

type bus struct {
	subscribers map[string][]func(message)
}

func (b *bus) subscribe(msgType string, h func(message)) {
	b.subscribers[msgType] = append(b.subscribers[msgType], h)
}

func (b *bus) publish(m message) {
	fmt.Printf("bus: %s order=%d amount_cents=%d\n", m.Type, m.Order, m.AmountCents)
	for _, h := range b.subscribers[m.Type] {
		h(m)
	}
}

// shopAdapter reads the format of the shop, "order 42 total 19.99".
func shopAdapter(b *bus, line string) error {
	f := strings.Fields(line)
	order, err := strconv.Atoi(f[1])
	if err != nil {
		return err
	}
	units, cents, _ := strings.Cut(f[3], ".")
	u, err := strconv.Atoi(units)
	if err != nil {
		return err
	}
	c, err := strconv.Atoi(cents)
	if err != nil {
		return err
	}
	b.publish(message{Type: "OrderPlaced", Order: order, AmountCents: u*100 + c})
	return nil
}

func main() {
	b := &bus{subscribers: map[string][]func(message){}}
	b.subscribe("OrderPlaced", func(m message) {
		fmt.Printf("billing: charge %d cents for order %d\n", m.AmountCents, m.Order)
	})
	b.subscribe("OrderPlaced", func(m message) {
		fmt.Printf("warehouse: SHIP|%d\n", m.Order)
	})
	send := func(line string) {
		fmt.Println("shop sends:", line)
		if err := shopAdapter(b, line); err != nil {
			fmt.Println("shop adapter:", err)
		}
	}
	send("order 42 total 19.99")

	fmt.Println("analytics joins the bus")
	seen := 0
	b.subscribe("OrderPlaced", func(m message) {
		seen++
		fmt.Printf("analytics: order %d counted, total orders %d\n", m.Order, seen)
	})
	send("order 43 total 5.00")
}
```

```output
shop sends: order 42 total 19.99
bus: OrderPlaced order=42 amount_cents=1999
billing: charge 1999 cents for order 42
warehouse: SHIP|42
analytics joins the bus
shop sends: order 43 total 5.00
bus: OrderPlaced order=43 amount_cents=500
billing: charge 500 cents for order 43
warehouse: SHIP|43
analytics: order 43 counted, total orders 1
```

- Verdict: idiom. Go has no message bus. A map from a message type to a slice of functions is the bus, and the struct `message` is the common data model.
- A struct for the common message lets the compiler check each adapter against the model. A change of a field breaks the build of each adapter that uses it.
- `shopAdapter` returns an error for text that it cannot read. On a real bus, it would send that text to an invalid message channel.
