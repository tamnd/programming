```go run id=goroutine-limit
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

func main() {
	const tasks = 1_000
	permits := make(chan struct{}, 10) // a buffered channel used as a semaphore
	var active, peak, sum atomic.Int64
	var wg sync.WaitGroup
	for i := range tasks {
		wg.Add(1)
		go func() {
			defer wg.Done()
			permits <- struct{}{}
			defer func() { <-permits }()
			now := active.Add(1)
			for {
				old := peak.Load()
				if now <= old || peak.CompareAndSwap(old, now) {
					break
				}
			}
			time.Sleep(10 * time.Millisecond)
			sum.Add(int64(i))
			active.Add(-1)
		}()
	}
	wg.Wait()
	fmt.Printf("tasks: %d, sum: %d\n", tasks, sum.Load())
	fmt.Println("never more than 10 at once:", peak.Load() <= 10)
}
```

```output for=goroutine-limit
tasks: 1000, sum: 499500
never more than 10 at once: true
```

- Verdict: native. Go has no type called a virtual thread, but goroutines are the same idea: the runtime schedules many goroutines onto a few OS threads.
- The program starts one goroutine for each task. A buffered channel with room for 10 values acts as the semaphore: a send takes a permit and a receive gives it back.
- A goroutine that waits on the full channel or in `time.Sleep` does not hold an OS thread.
- The loop with `CompareAndSwap` keeps the largest number of active tasks that any goroutine has seen.
