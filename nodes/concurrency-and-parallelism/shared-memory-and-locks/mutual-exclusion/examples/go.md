```go run id=counter
package main

import (
	"fmt"
	"sync"
)

type HitCounter struct {
	mu   sync.RWMutex
	hits map[string]int
}

func (c *HitCounter) Add(page string) {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.hits[page]++
}

func (c *HitCounter) Get(page string) int {
	c.mu.RLock()
	defer c.mu.RUnlock()
	return c.hits[page]
}

func main() {
	counter := &HitCounter{hits: map[string]int{}}
	var wg sync.WaitGroup
	for range 4 {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for range 10_000 {
				counter.Add("/home")
			}
		}()
	}
	wg.Wait()
	fmt.Println("hits:", counter.Get("/home"))
}
```

```output
hits: 40000
```

- Verdict: stdlib. The package `sync` gives `Mutex` and `RWMutex`. The zero value of each is an unlocked lock, so the struct needs no constructor for it.
- `defer c.mu.Unlock()` releases the lock when the method returns, also during a panic. Put it on the line after `Lock`.
- A mutex must not be copied after its first use. Give methods a pointer receiver, as `*HitCounter` here. The command `go vet` reports copies of locks.
- A Go map is not safe for concurrent writes, so the map needs the lock. `RWMutex` lets many `Get` calls run at once, and a waiting writer blocks further readers.
