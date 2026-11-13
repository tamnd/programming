```go run id=three-ways
package main

import (
	"fmt"
	"sync"
)

const (
	goroutines = 4
	steps      = 1000
)

func runAll(body func(i int)) {
	var wg sync.WaitGroup
	for i := 0; i < goroutines; i++ {
		wg.Add(1)
		go func(i int) {
			defer wg.Done()
			body(i)
		}(i)
	}
	wg.Wait()
}

func main() {
	// 1. Synchronization: a mutex guards the shared counter.
	count := 0
	var mu sync.Mutex
	runAll(func(int) {
		for n := 0; n < steps; n++ {
			mu.Lock()
			count++
			mu.Unlock()
		}
	})
	fmt.Println("lock around a shared counter:", count)

	// 2. Confinement: one owner goroutine keeps the counter; the others send it messages.
	inbox := make(chan int)
	owned := make(chan int)
	go func() {
		total := 0
		for n := 0; n < goroutines*steps; n++ {
			total += <-inbox
		}
		owned <- total
	}()
	runAll(func(int) {
		for n := 0; n < steps; n++ {
			inbox <- 1
		}
	})
	fmt.Println("one owner thread, the others send messages:", <-owned)

	// 3. No sharing: each goroutine makes its own result, and the results are combined after the wait.
	parts := make([]int, goroutines)
	runAll(func(i int) {
		for n := 0; n < steps; n++ {
			parts[i]++ // only goroutine i writes slot i
		}
	})
	sum := 0
	for _, p := range parts {
		sum += p
	}
	fmt.Println("own result for each thread, combined after the join:", sum)
}
```

```output
lock around a shared counter: 4000
one owner thread, the others send messages: 4000
own result for each thread, combined after the join: 4000
```

- Verdict: native. A `sync.Mutex` for the shared counter, an unbuffered channel to the owner goroutine, and a slice slot for each goroutine, with a `sync.WaitGroup` for the join.
