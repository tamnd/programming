```go run id=many-goroutines
package main

import (
	"fmt"
	"runtime"
)

func main() {
	runtime.GOMAXPROCS(1) // one OS thread runs Go code at a time
	const count = 100_000
	results := make(chan int)
	for i := range count {
		go func() {
			results <- i
		}()
	}
	sum := 0
	for range count {
		sum += <-results
	}
	fmt.Printf("goroutines: %d, GOMAXPROCS: %d, sum: %d\n", count, runtime.GOMAXPROCS(0), sum)
}
```

```output for=many-goroutines
goroutines: 100000, GOMAXPROCS: 1, sum: 4999950000
```

- Verdict: native. Goroutines are green threads: the Go runtime schedules them onto OS threads, and a `go` statement starts one.
- With `GOMAXPROCS(1)`, only one OS thread runs Go code at a time. The 100,000 goroutines still all run, because each one blocks on the channel send, and the runtime then switches to another goroutine.
- Each goroutine starts with a small stack that grows when needed, so 100,000 of them fit in a modest amount of memory.
- `runtime.GOMAXPROCS(0)` reads the setting without a change.
