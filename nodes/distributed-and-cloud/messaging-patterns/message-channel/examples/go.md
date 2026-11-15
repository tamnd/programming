```go run id=named-channels
package main

import (
	"fmt"
	"sync"
)

// Channels maps a name to a buffered Go channel.
// Senders and receivers share only the names.
type Channels struct{ byName map[string]chan string }

func NewChannels(capacity int, names ...string) *Channels {
	c := &Channels{byName: map[string]chan string{}}
	for _, n := range names {
		c.byName[n] = make(chan string, capacity)
	}
	return c
}

func (c *Channels) Send(name, message string) error {
	ch, ok := c.byName[name]
	if !ok {
		return fmt.Errorf("no channel named %s", name)
	}
	ch <- message
	return nil
}

func (c *Channels) Depth(name string) int { return len(c.byName[name]) }

// Close tells the receivers that no more messages will come.
func (c *Channels) Close() {
	for _, ch := range c.byName {
		close(ch)
	}
}

func main() {
	channels := NewChannels(10, "orders", "payments")
	channels.Send("orders", "order 1: 2 x A-7")
	channels.Send("orders", "order 2: 1 x B-1")
	channels.Send("payments", "payment for order 1: 30.00")
	fmt.Printf("waiting: orders=%d payments=%d\n", channels.Depth("orders"), channels.Depth("payments"))
	channels.Close()

	for _, r := range []struct{ name, who string }{{"orders", "warehouse"}, {"payments", "accounting"}} {
		var wg sync.WaitGroup
		wg.Add(1)
		go func() {
			defer wg.Done()
			for m := range channels.byName[r.name] {
				fmt.Printf("%s: %s\n", r.who, m)
			}
		}()
		wg.Wait()
	}

	if err := channels.Send("order", "order 3: 1 x C-2"); err != nil {
		fmt.Println("error:", err)
	}
}
```

```output
waiting: orders=2 payments=1
warehouse: order 1: 2 x A-7
warehouse: order 2: 1 x B-1
accounting: payment for order 1: 30.00
error: no channel named order
```

- Verdict: native. A Go channel is a typed channel between goroutines, and `Channels` gives each one a name. The type `chan string` is the Datatype Channel idea in the type system: a channel carries values of one type.
- The channels have a buffer of 10, so the sends do not block before a receiver runs. With no buffer, a send waits until a receiver is ready.
- `Close` tells the receivers that no more messages will come, and the `range` loops then end. A broker channel does not close in this way: a consumer waits for the next message, or stops on a timeout.
