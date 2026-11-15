```go run id=dispatch-commands
package main

import (
	"encoding/json"
	"fmt"
)

// envelope is the form of each command message: the name of the command and its arguments.
type envelope struct {
	Command string          `json:"command"`
	Args    json.RawMessage `json:"args"`
}

type reserveStock struct {
	SKU string `json:"sku"`
	Qty int    `json:"qty"`
}

type chargeCard struct {
	Order int `json:"order"`
	Cents int `json:"cents"`
}

var stock = map[string]int{"A1": 5}

func handle(raw []byte) (string, error) {
	var env envelope
	if err := json.Unmarshal(raw, &env); err != nil {
		return "", err
	}
	switch env.Command {
	case "ReserveStock":
		var c reserveStock
		if err := json.Unmarshal(env.Args, &c); err != nil {
			return "", err
		}
		if stock[c.SKU] < c.Qty {
			return fmt.Sprintf("reserve %d of %s: not enough stock, %d left", c.Qty, c.SKU, stock[c.SKU]), nil
		}
		stock[c.SKU] -= c.Qty
		return fmt.Sprintf("reserve %d of %s: %d left", c.Qty, c.SKU, stock[c.SKU]), nil
	case "ChargeCard":
		var c chargeCard
		if err := json.Unmarshal(env.Args, &c); err != nil {
			return "", err
		}
		return fmt.Sprintf("charge %d cents for order %d: done", c.Cents, c.Order), nil
	}
	return "", fmt.Errorf("unknown command %q", env.Command)
}

func encode(command string, args any) []byte {
	a, err := json.Marshal(args)
	if err != nil {
		panic(err)
	}
	raw, err := json.Marshal(envelope{Command: command, Args: a})
	if err != nil {
		panic(err)
	}
	return raw
}

func main() {
	channel := make(chan []byte, 3) // the channel carries bytes
	channel <- encode("ReserveStock", reserveStock{SKU: "A1", Qty: 2})
	channel <- encode("ChargeCard", chargeCard{Order: 42, Cents: 1999})
	channel <- encode("ReserveStock", reserveStock{SKU: "A1", Qty: 9})
	close(channel)
	for raw := range channel {
		out, err := handle(raw)
		if err != nil {
			fmt.Println("invalid message:", err)
			continue
		}
		fmt.Println(out)
	}
}
```

```output
reserve 2 of A1: 3 left
charge 1999 cents for order 42: done
reserve 9 of A1: not enough stock, 3 left
```

- Verdict: stdlib. `encoding/json` encodes an envelope with the name of the command and its arguments, and `json.RawMessage` keeps the arguments as bytes until the receiver knows their type.
- The `switch` on `env.Command` is the dispatcher. An unknown name gives an error, and the loop reports it as an invalid message.
- The channel has a buffer of 3, so `main` can send the three commands before it reads them, in one goroutine.
