```go run id=dcl
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
)

type Config struct{}

var (
	created     atomic.Int32
	lockedCalls int
	instancePtr atomic.Pointer[Config]
	mu          sync.Mutex
)

func instance() *Config {
	if p := instancePtr.Load(); p != nil { // first check, no lock
		return p
	}
	mu.Lock()
	defer mu.Unlock()
	lockedCalls++
	if p := instancePtr.Load(); p != nil { // second check, lock held
		return p
	}
	created.Add(1)
	p := &Config{}
	instancePtr.Store(p)
	return p
}

func main() {
	var ready, wg sync.WaitGroup
	start := make(chan struct{})
	for w := 0; w < 8; w++ {
		wg.Add(1)
		ready.Add(1)
		go func() {
			defer wg.Done()
			ready.Done()
			<-start
			for i := 0; i < 1000; i++ {
				instance()
			}
		}()
	}
	ready.Wait()
	close(start)
	wg.Wait()
	fmt.Println("instances created:", created.Load())
	fmt.Println("calls:", 8*1000)
	if lockedCalls <= 8 {
		fmt.Println("calls that took the lock: 8 or fewer")
	} else {
		fmt.Println("calls that took the lock:", lockedCalls)
	}
}
```

```output
instances created: 1
calls: 8000
calls that took the lock: 8 or fewer
```

- Verdict: idiom. The pointer is an `atomic.Pointer`, so the first check is a safe read. `sync.Once` is the library form of the same work.
