```go run id=atomic
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
)

func main() {
	var hits atomic.Int64
	var wg sync.WaitGroup
	for range 4 {
		wg.Go(func() {
			for range 100_000 {
				hits.Add(1)
			}
		})
	}
	wg.Wait()
	fmt.Println(hits.Load())

	fmt.Println(hits.CompareAndSwap(400_000, 0), hits.Load())

	var ready atomic.Bool
	var message string
	go func() {
		message = "hello"
		ready.Store(true)
	}()
	for !ready.Load() {
	}
	fmt.Println(message)
}
```

```output for=atomic
400000
true 0
hello
```

- Verdict: stdlib. `sync/atomic` has the types `Int32`, `Int64`, `Uint64`, `Bool`, `Pointer[T]` and `Value`. All atomic operations in Go are sequentially consistent, so the write to `message` before `ready.Store` is visible after `ready.Load` returns true.
- The Go memory model recommends channels and mutexes first. The race detector, `go run -race`, finds data races at run time.
