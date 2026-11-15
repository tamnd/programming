```go run id=price-requests
package main

import (
	"fmt"
	"time"
)

type message struct {
	ID, ReplyTo, CorrelationID, Body string
}

var prices = map[string]int{"A1": 1999, "B2": 500}

// Named channels, as on a broker: one request channel and one reply channel for each requestor.
var channels = map[string]chan message{
	"prices":         make(chan message),
	"replies.web":    make(chan message, 1),
	"replies.mobile": make(chan message, 1),
}

func replier(done chan<- struct{}) {
	for request := range channels["prices"] {
		body := fmt.Sprintf("error unknown sku %s", request.Body)
		if p, ok := prices[request.Body]; ok {
			body = fmt.Sprintf("%d cents", p)
		}
		channels[request.ReplyTo] <- message{CorrelationID: request.ID, Body: body} // the return address
	}
	close(done)
}

func ask(requestor, id, sku string) string {
	replyTo := "replies." + requestor
	channels["prices"] <- message{ID: id, ReplyTo: replyTo, Body: sku}
	select { // block, but not for ever
	case reply := <-channels[replyTo]:
		return fmt.Sprintf("%s asked for %s: %s", requestor, sku, reply.Body)
	case <-time.After(2 * time.Second):
		return fmt.Sprintf("%s asked for %s: no reply in 2 s", requestor, sku)
	}
}

func main() {
	done := make(chan struct{})
	go replier(done)
	fmt.Println(ask("web", "q1", "A1"))
	fmt.Println(ask("mobile", "q2", "B2"))
	fmt.Println(ask("web", "q3", "Z9"))
	close(channels["prices"]) // stop the replier
	<-done
}
```

```output
web asked for A1: 1999 cents
mobile asked for B2: 500 cents
web asked for Z9: error unknown sku Z9
```

- Verdict: idiom. Go has no request-reply channel, but channels and a goroutine give the two channels and the replier.
- `select` with `time.After` waits for the reply or for 2 seconds, whichever comes first.
- In one Go program, the return address can also be a channel value in the request, as in the Go example of the Return Address page; here it is a name, as on a broker.
