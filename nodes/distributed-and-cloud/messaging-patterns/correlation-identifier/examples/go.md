```go run id=match-replies
package main

import (
	"encoding/json"
	"fmt"
)

type request struct {
	ID  string `json:"id"`
	SKU string `json:"sku"`
}

type reply struct {
	CorrelationID string `json:"correlation_id"`
	Body          string `json:"body"`
}

var prices = map[string]int{"A1": 1999, "B2": 500}

func encode(v any) []byte {
	raw, err := json.Marshal(v)
	if err != nil {
		panic(err)
	}
	return raw
}

func main() {
	// One reply channel for all the requests of this requestor.
	replyChannel := [][]byte{encode(reply{"q0", "1999 cents"})} // left from a request that timed out

	pending := map[string]string{} // request id -> what the request asked for
	var requests [][]byte
	for _, r := range []request{{"q1", "A1"}, {"q2", "B2"}, {"q3", "Z9"}} {
		pending[r.ID] = r.SKU
		requests = append(requests, encode(r))
	}

	// The replier works on the requests in a different order, and it copies the id of each request.
	for i := len(requests) - 1; i >= 0; i-- {
		var r request
		if err := json.Unmarshal(requests[i], &r); err != nil {
			panic(err)
		}
		body := fmt.Sprintf("error unknown sku %s", r.SKU)
		if p, ok := prices[r.SKU]; ok {
			body = fmt.Sprintf("%d cents", p)
		}
		replyChannel = append(replyChannel, encode(reply{r.ID, body}))
	}

	for _, raw := range replyChannel {
		var r reply
		if err := json.Unmarshal(raw, &r); err != nil {
			panic(err)
		}
		sku, ok := pending[r.CorrelationID]
		if !ok {
			fmt.Printf("no outstanding request for %s, dropped\n", r.CorrelationID)
			continue
		}
		delete(pending, r.CorrelationID)
		fmt.Printf("reply for %s (%s): %s\n", r.CorrelationID, sku, r.Body)
	}
	fmt.Println("still waiting:", len(pending))
}
```

```output
no outstanding request for q0, dropped
reply for q3 (Z9): error unknown sku Z9
reply for q2 (B2): 500 cents
reply for q1 (A1): 1999 cents
still waiting: 0
```

- Verdict: idiom. A `map[string]string` from the request id to the item is the table, and `encoding/json` gives the bytes of each message.
- The form `sku, ok := pending[id]` tells a missing entry from an empty value, so a stray reply is found.
- With many goroutines that read replies, the map needs a `sync.Mutex`, as the RabbitMQ Java client uses `synchronized` on its table.
