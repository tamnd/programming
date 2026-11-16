```go run id=threads-and-the-gil
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
)

func main() {
	// Go has no global interpreter lock: up to GOMAXPROCS goroutines run Go code at the same time.
	// So a shared counter must be atomic, or guarded by a mutex.
	var total atomic.Int64
	var wg sync.WaitGroup
	for range 4 {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for range 100_000 {
				total.Add(1)
			}
		}()
	}
	wg.Wait()
	fmt.Println("4 goroutines added 100000 each with atomic.Int64")
	fmt.Println("total:", total.Load())
}
```

```output for=threads-and-the-gil
4 goroutines added 100000 each with atomic.Int64
total: 400000
```

- Verdict: absent. Go has no GIL. The `GOMAXPROCS` variable "limits the number of operating system threads that can execute user-level Go code simultaneously". By default, the runtime picks it from the number of logical CPUs of the machine and the limits of the process.
- `atomic.Int64` makes each `Add` one atomic step. A plain `int64` with `total++` in each goroutine would be a data race.
