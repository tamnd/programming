```go run id=copy-on-write
package main

import (
	"fmt"
	"strconv"
	"sync"
	"sync/atomic"
)

// CopyOnWriteList keeps a slice that is never changed after it is published.
type CopyOnWriteList struct {
	items     atomic.Pointer[[]string]
	writeLock sync.Mutex // writers only
}

func (l *CopyOnWriteList) Add(item string) {
	l.writeLock.Lock()
	defer l.writeLock.Unlock()
	old := *l.items.Load()
	next := make([]string, len(old), len(old)+1)
	copy(next, old) // copy, then change the copy
	next = append(next, item)
	l.items.Store(&next)
}

func (l *CopyOnWriteList) Snapshot() []string { return *l.items.Load() }

func main() {
	listeners := &CopyOnWriteList{}
	initial := []string{"listener 0"}
	listeners.items.Store(&initial)
	first := listeners.Snapshot()
	var iterations, errors atomic.Int64
	var wg sync.WaitGroup
	for i := 0; i < 4; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for r := 0; r < 1000; r++ {
				for range listeners.Snapshot() {
					// call each listener
				}
				iterations.Add(1)
			}
		}()
	}
	wg.Add(1)
	go func() {
		defer wg.Done()
		for n := 1; n <= 100; n++ {
			listeners.Add("listener " + strconv.Itoa(n))
		}
	}()
	wg.Wait()
	fmt.Println("iterations:", iterations.Load())
	fmt.Println("iterations that failed:", errors.Load())
	fmt.Println("listeners at the end:", len(listeners.Snapshot()))
	fmt.Println("length of the first snapshot:", len(first))
}
```

```output
iterations: 4000
iterations that failed: 0
listeners at the end: 101
length of the first snapshot: 1
```

- Verdict: idiom. `CopyOnWriteList` copies the slice in `Add` and publishes the copy with `atomic.Pointer`; a range loop over a snapshot cannot fail, so `errors` stays 0.
