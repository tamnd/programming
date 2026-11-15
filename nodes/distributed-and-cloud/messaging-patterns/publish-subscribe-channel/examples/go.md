```go run id=three-subscribers
package main

import (
	"fmt"
	"strings"
	"sync"
)

// Topic is a publish-subscribe channel: one Go channel for each subscriber.
type Topic struct {
	names []string
	subs  map[string]chan string
}

func NewTopic() *Topic { return &Topic{subs: map[string]chan string{}} }

func (t *Topic) Subscribe(name string) <-chan string {
	ch := make(chan string, 16)
	t.names = append(t.names, name)
	t.subs[name] = ch
	return ch
}

// Publish sends a copy of the event to each subscriber and returns the number of copies.
func (t *Topic) Publish(event string) int {
	for _, ch := range t.subs {
		ch <- event
	}
	return len(t.subs)
}

func (t *Topic) Close() {
	for _, ch := range t.subs {
		close(ch)
	}
}

func main() {
	topic := NewTopic()
	seen := map[string][]string{}
	var mu sync.Mutex
	var wg sync.WaitGroup
	for _, name := range []string{"billing", "email", "stock"} {
		inbox := topic.Subscribe(name)
		wg.Add(1)
		go func() {
			defer wg.Done()
			var got []string
			for event := range inbox {
				got = append(got, event)
			}
			mu.Lock()
			seen[name] = got
			mu.Unlock()
		}()
	}

	copies := 0
	for n := 1; n <= 3; n++ {
		copies += topic.Publish(fmt.Sprintf("order %d placed", n))
	}
	topic.Close()
	wg.Wait()

	for _, name := range topic.names {
		fmt.Printf("%s: %s\n", name, strings.Join(seen[name], ", "))
	}
	fmt.Println("copies sent:", copies)
}
```

```output
billing: order 1 placed, order 2 placed, order 3 placed
email: order 1 placed, order 2 placed, order 3 placed
stock: order 1 placed, order 2 placed, order 3 placed
copies sent: 9
```

- Verdict: idiom. Go has no broadcast channel. A value sent on a channel goes to one receiver, so `Topic` keeps one channel for each subscriber and sends a copy to each.
- Each channel has a buffer of 16, so `Publish` does not wait for a slow subscriber until its buffer is full. A full buffer then blocks the publisher, which is the cost of a topic in memory with no broker.
- `topic.names` keeps the order of subscription, because the order of a Go map is not specified.
