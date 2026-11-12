```go run id=thread-pool
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

type job struct {
	n      int
	result chan int
}

var running, mostRunning atomic.Int64

func task(n int) int {
	now := running.Add(1)
	for {
		seen := mostRunning.Load()
		if seen >= now || mostRunning.CompareAndSwap(seen, now) {
			break
		}
	}
	time.Sleep(time.Millisecond)
	running.Add(-1)
	return n * n
}

func main() {
	jobs := make(chan job) // the work queue
	var mu sync.Mutex
	workersUsed := map[int]bool{}
	var wg sync.WaitGroup
	for id := 0; id < 4; id++ { // a fixed number of worker goroutines
		wg.Add(1)
		go func(id int) {
			defer wg.Done()
			for j := range jobs {
				mu.Lock()
				workersUsed[id] = true
				mu.Unlock()
				j.result <- task(j.n)
			}
		}(id)
	}

	results := make([]chan int, 100)
	for n := range results {
		results[n] = make(chan int, 1)
	}
	go func() {
		for n := range results {
			jobs <- job{n, results[n]}
		}
		close(jobs)
	}()

	sum := 0
	for _, r := range results {
		sum += <-r
	}
	wg.Wait()
	fmt.Println("tasks run:", len(results))
	fmt.Println("sum of results:", sum)
	fmt.Println("workers used, at most 4:", boolText(len(workersUsed) <= 4))
	fmt.Println("most tasks running at once, at most 4:", boolText(mostRunning.Load() <= 4))
}

func boolText(b bool) string {
	if b {
		return "True"
	}
	return "False"
}
```

```output
tasks run: 100
sum of results: 328350
workers used, at most 4: True
most tasks running at once, at most 4: True
```

- Verdict: idiom. A fixed number of goroutines read from the `jobs` channel, the form that Effective Go gives, and each job carries a channel for its result.
