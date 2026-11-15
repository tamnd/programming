```go run id=max-deliveries
package main

import (
	"errors"
	"fmt"
	"strconv"
	"strings"
)

const maxDeliveries = 3

type message struct {
	body       string
	deliveries int
	reason     string
	source     string
}

func handle(body string) error {
	f := strings.Fields(body)
	if _, err := strconv.Atoi(f[1]); err != nil {
		return errors.New("bad amount " + f[1])
	}
	fmt.Printf("ok %s amount %s\n", f[0], f[1])
	return nil
}

func main() {
	var queue []*message
	for _, b := range []string{"pay-1 10", "pay-2 x", "pay-3 5"} {
		queue = append(queue, &message{body: b})
	}
	var deadLetters []*message
	processed := 0
	for len(queue) > 0 {
		msg := queue[0]
		queue = queue[1:]
		msg.deliveries++
		err := handle(msg.body)
		if err == nil {
			processed++
			continue
		}
		name := strings.Fields(msg.body)[0]
		fmt.Printf("fail %s delivery %d: %v\n", name, msg.deliveries, err)
		if msg.deliveries < maxDeliveries {
			queue = append(queue, msg)
		} else {
			msg.reason, msg.source = err.Error(), "payments"
			deadLetters = append(deadLetters, msg)
		}
	}
	for _, msg := range deadLetters {
		fmt.Printf("dead letter %s: deliveries=%d reason=%s source=%s\n",
			strings.Fields(msg.body)[0], msg.deliveries, msg.reason, msg.source)
	}
	fmt.Printf("processed %d, dead letters %d\n", processed, len(deadLetters))
}
```

```output
ok pay-1 amount 10
fail pay-2 delivery 1: bad amount x
ok pay-3 amount 5
fail pay-2 delivery 2: bad amount x
fail pay-2 delivery 3: bad amount x
dead letter pay-2: deliveries=3 reason=bad amount x source=payments
processed 2, dead letters 1
```

- Verdict: idiom. Go has no dead letter channel. A slice of pointers is the queue, a field of the message is the delivery count, and a second slice is the dead letter channel.
- `handle` returns an `error`, and the loop decides from the error and the count. The reason that is kept with the dead letter is the text of the error.
- A slice is used in place of a channel because the loop puts messages back into the queue that it reads. With a buffered channel, a send back into a full channel from its only reader would block for ever.
