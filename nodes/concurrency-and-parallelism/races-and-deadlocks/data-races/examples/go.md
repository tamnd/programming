```go run id=atomic
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
)

func main() {
	var count atomic.Int64
	var wg sync.WaitGroup
	for range 2 {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for range 100_000 {
				count.Add(1)
			}
		}()
	}
	wg.Wait()
	fmt.Println(count.Load())
}
```

```output for=atomic
200000
```

```go fails expect=exit:66 flags="-race" id=race
package main

import "sync"

func main() {
	count := 0
	var wg sync.WaitGroup
	for range 2 {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for range 1000 {
				count++
			}
		}()
	}
	wg.Wait()
}
```

- Verdict: stdlib. `sync/atomic` gives the type `atomic.Int64`, whose `Add` is one atomic read-modify-write.
- The second program has a plain `count++` in two goroutines. `go run -race` reports `WARNING: DATA RACE` and the program exits with status 66, the default exit code of the detector.
- The race detector needs cgo. On systems other than Darwin, it also needs a C compiler.
- A race on a single word gives some written value. A race on an interface, map, slice or string can corrupt memory.
