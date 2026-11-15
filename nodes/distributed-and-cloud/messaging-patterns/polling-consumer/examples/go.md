```go run id=poll-orders
package main

import (
	"fmt"
	"strings"
	"time"
)

var orders = make(chan string, 16)

// poll waits up to timeout for one message, then takes up to limit messages.
func poll(limit int, timeout time.Duration) []string {
	var batch []string
	if timeout == 0 {
		select {
		case m := <-orders:
			batch = append(batch, m)
		default:
			return nil
		}
	} else {
		select {
		case m := <-orders:
			batch = append(batch, m)
		case <-time.After(timeout):
			return nil
		}
	}
	for len(batch) < limit {
		select {
		case m := <-orders:
			batch = append(batch, m)
		default:
			return batch
		}
	}
	return batch
}

func main() {
	if b := poll(2, 0); b == nil {
		fmt.Println("poll 0, no wait: empty")
	}
	for n := 1; n <= 5; n++ {
		orders <- fmt.Sprintf("order-%d", n)
	}
	fmt.Println("producer: 5 orders on the queue")

	for n := 1; ; n++ {
		if n == 4 {
			// a producer sends one more order while the consumer waits
			time.AfterFunc(50*time.Millisecond, func() { orders <- "order-6" })
		}
		batch := poll(2, 300*time.Millisecond)
		if batch == nil {
			fmt.Printf("poll %d: nothing in 300 ms, the consumer stops\n", n)
			break
		}
		fmt.Printf("poll %d: %s (%d left)\n", n, strings.Join(batch, " "), len(orders))
		// the consumer works on the batch here, at its own pace
	}
}
```

```output
poll 0, no wait: empty
producer: 5 orders on the queue
poll 1: order-1 order-2 (3 left)
poll 2: order-3 order-4 (1 left)
poll 3: order-5 (0 left)
poll 4: order-6 (0 left)
poll 5: nothing in 300 ms, the consumer stops
```

- Verdict: idiom. A `select` with a `default` case is a receive with no wait, and a `select` with `time.After` is a receive with a timeout.
- `len(orders)` gives the number of values in the buffer of the channel, which stands for the depth of the queue.
- `time.AfterFunc` plays a producer that sends while the consumer waits in poll 4.
