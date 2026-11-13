```go run id=pipeline-concurrency
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
)

const (
	items   = 20
	workers = 3
)

var sinkStarted atomic.Bool

// source sends the numbers and closes its channel when it is done.
func source(overlap *bool) <-chan int {
	out := make(chan int, 1) // blocks while the buffer is full
	go func() {
		for n := 1; n <= items; n++ {
			out <- n
		}
		// Each stage can hold only a few values, so the sink must already be running.
		*overlap = sinkStarted.Load()
		close(out)
	}()
	return out
}

// square starts the workers on one input channel (fan-out). They send to one
// output channel, which is closed when all workers are done (fan-in).
func square(in <-chan int) <-chan int {
	out := make(chan int, 1)
	var wg sync.WaitGroup
	for w := 0; w < workers; w++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for n := range in {
				out <- n * n
			}
		}()
	}
	go func() {
		wg.Wait()
		close(out)
	}()
	return out
}

func main() {
	var overlap bool
	squares := square(source(&overlap))
	received, total := 0, 0
	for v := range squares {
		sinkStarted.Store(true)
		received++
		total += v
	}

	fmt.Println("items from the source:", items)
	fmt.Println("square workers:", workers)
	fmt.Println("squares that reached the sink:", received)
	fmt.Println("sum of squares:", total)
	fmt.Println("the sink got a value before the source sent its last:", map[bool]string{true: "True", false: "False"}[overlap])
}
```

```output
items from the source: 20
square workers: 3
squares that reached the sink: 20
sum of squares: 2870
the sink got a value before the source sent its last: True
```

- Verdict: native. Buffered channels link the stages, `range` reads until a channel is closed, and a `sync.WaitGroup` closes the output when all workers are done.
