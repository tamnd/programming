```go run id=best-quote
package main

import (
	"fmt"
	"sort"
	"time"
)

type quote struct {
	supplier string
	price    int
}

func main() {
	// Each supplier replies after some time with a price in cents.
	delays := map[string]time.Duration{"alpha": 20 * time.Millisecond, "beta": 80 * time.Millisecond, "gamma": time.Second}
	prices := map[string]int{"alpha": 1250, "beta": 1190, "gamma": 1100}

	replies := make(chan quote, len(delays)) // buffered, so a late sender does not block
	for name := range delays {               // scatter
		go func(name string) {
			time.Sleep(delays[name]) // a stand-in for a request on the channel of the supplier
			replies <- quote{name, prices[name]}
		}(name)
	}

	fmt.Printf("asked %d suppliers, wait at most 400 ms\n", len(delays))
	got := map[string]int{}
	deadline := time.After(400 * time.Millisecond)
gather:
	for len(got) < len(delays) {
		select {
		case q := <-replies:
			got[q.supplier] = q.price
		case <-deadline:
			break gather
		}
	}

	names := make([]string, 0, len(delays))
	for name := range delays {
		names = append(names, name)
	}
	sort.Strings(names)
	best := ""
	for _, name := range names {
		price, ok := got[name]
		if !ok {
			fmt.Printf("no reply from %s in time\n", name)
			continue
		}
		fmt.Printf("reply from %s: %d\n", name, price)
		if best == "" || price < got[best] {
			best = name
		}
	}
	fmt.Printf("best: %s at %d\n", best, got[best])
}
```

```output
asked 3 suppliers, wait at most 400 ms
reply from alpha: 1250
reply from beta: 1190
no reply from gamma in time
best: beta at 1190
```

- Verdict: idiom. One goroutine for each supplier scatters the requests, and a `select` on the channel of the replies and on `time.After` gathers them until the deadline.
- The channel has a buffer for all the replies, so a goroutine that replies after the deadline does not block for ever.
- In a server, a `context.WithTimeout` gives the same deadline and also tells the goroutines to stop.
