```go run id=stop-a-thread
package main

import (
	"context"
	"fmt"
	"time"
)

func worker(ctx context.Context, done chan<- struct{}) {
	defer close(done)
	for {
		select {
		case <-ctx.Done():
			fmt.Println("worker: stopped because", ctx.Err())
			return
		case <-time.After(10 * time.Second):
			// One step of work would go here.
		}
	}
}

func main() {
	ctx, cancel := context.WithCancel(context.Background())
	done := make(chan struct{})
	go worker(ctx, done)

	time.Sleep(100 * time.Millisecond)
	start := time.Now()
	cancel()
	<-done
	fmt.Println("worker stopped in under 1 s:", time.Since(start) < time.Second)
}
```

```output for=stop-a-thread
worker: stopped because context canceled
worker stopped in under 1 s: true
```

- Verdict: stdlib. A `context.Context` carries "cancellation signals". The goroutine checks the context and returns by itself.
- `context.WithCancel` returns a context and a cancel function. The context's `Done` channel "is closed when the returned cancel function is called". The `select` in the worker returns when that channel closes.
- `ctx.Err()` gives the reason for the stop. Here it prints `context canceled`.
- "When a Context is canceled, all Contexts derived from it are also canceled", so one `cancel()` can stop a tree of goroutines.
