```go run id=hsha
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

func main() {
	requests := make(chan int, 100) // the queueing layer: a buffered channel
	gate := make(chan struct{})     // lets the demo hold the sync layer
	var done, sum atomic.Int64
	received := 0

	var wg sync.WaitGroup
	for i := 0; i < 4; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			<-gate
			for request := range requests { // the sync layer blocks here
				time.Sleep(time.Millisecond) // a blocking call, such as a database query
				done.Add(1)
				sum.Add(int64(request))
			}
		}()
	}

	// Async layer: a short handler that never blocks while the buffer has room.
	onEvent := func(eventID int) {
		received++
		requests <- eventID
	}
	sourceDone := make(chan struct{})
	go func() {
		for id := 0; id < 100; id++ {
			onEvent(id)
		}
		close(sourceDone)
	}()
	<-sourceDone
	fmt.Println("events received by the async layer:", received)
	fmt.Println("requests in the queueing layer before the sync layer starts:", len(requests))
	close(gate)
	close(requests)
	wg.Wait()
	fmt.Println("requests done by the sync layer:", done.Load())
	fmt.Println("sum of request ids:", sum.Load())
}
```

```output
events received by the async layer: 100
requests in the queueing layer before the sync layer starts: 100
requests done by the sync layer: 100
sum of request ids: 4950
```

- Verdict: native. A buffered channel is the queueing layer: a send blocks only when the buffer is full, and `range` over the channel blocks each worker until a value arrives or the channel is closed.
