```go run id=goroutine-leak
package main

import (
	"context"
	"fmt"
	"runtime"
	"time"
)

// Leaks: only the first send has a receiver. The other workers block forever.
func firstLeaky(n int) int {
	results := make(chan int)
	for i := range n {
		go func() { results <- i * i }()
	}
	return <-results
}

// Fixed: the deferred cancel ends the workers that lost the race.
func firstWithContext(n int) int {
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	results := make(chan int)
	for i := range n {
		go func() {
			select {
			case results <- i * i:
			case <-ctx.Done():
			}
		}()
	}
	return <-results
}

// waitFor polls until want goroutines exist or the deadline passes.
func waitFor(want int) int {
	deadline := time.Now().Add(5 * time.Second)
	for runtime.NumGoroutine() != want && time.Now().Before(deadline) {
		time.Sleep(10 * time.Millisecond)
	}
	return runtime.NumGoroutine()
}

func main() {
	base := runtime.NumGoroutine()

	firstLeaky(10)
	fmt.Println("leaked goroutines:", waitFor(base+9)-base)

	before := runtime.NumGoroutine()
	firstWithContext(10)
	fmt.Println("leaked goroutines with context:", waitFor(before)-before)
}
```

```output
leaked goroutines: 9
leaked goroutines with context: 0
```

- Verdict: native. The collector frees unreachable memory, but it does not stop a goroutine. A blocked goroutine keeps its stack and each value that the stack refers to.
- `firstLeaky` returns after one receive. Nine workers then wait to send on a channel that no code reads.
- `waitFor` polls with a deadline, because the workers start and stop on the schedule of the runtime.
- In a running service, the `goroutine` profile of `net/http/pprof` groups goroutines by stack. A count that grows at one stack points to a leak.
