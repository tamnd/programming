```go run id=leader-followers
package main

import (
	"fmt"
	"sync"
	"time"
)

// ThreadPool lets one goroutine at a time wait on the handle set.
type ThreadPool struct {
	mu            sync.Mutex
	synchronizer  *sync.Cond
	leaderPresent bool
	waiting       int
	mostWaiting   int
	processed     int
	sum           int
}

func (p *ThreadPool) join(handleSet chan int) {
	for {
		p.mu.Lock() // follower role: wait for the turn to lead
		for p.leaderPresent {
			p.synchronizer.Wait()
		}
		p.leaderPresent = true
		p.waiting++
		p.mostWaiting = max(p.mostWaiting, p.waiting)
		p.mu.Unlock()

		event := <-handleSet // leader role: wait for an event
		p.promoteNewLeader()
		if event < 0 {
			return
		}
		time.Sleep(time.Millisecond) // processing role: handle the event here
		p.mu.Lock()
		p.processed++
		p.sum += event
		p.mu.Unlock()
	}
}

func (p *ThreadPool) promoteNewLeader() {
	p.mu.Lock()
	p.waiting--
	p.leaderPresent = false
	p.mu.Unlock()
	p.synchronizer.Signal()
}

func main() {
	handleSet := make(chan int, 104) // stands for the handle set
	for id := 0; id < 100; id++ {
		handleSet <- id
	}
	for i := 0; i < 4; i++ {
		handleSet <- -1 // one stop event per goroutine
	}
	pool := &ThreadPool{}
	pool.synchronizer = sync.NewCond(&pool.mu)
	var wg sync.WaitGroup
	for i := 0; i < 4; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			pool.join(handleSet)
		}()
	}
	wg.Wait()
	fmt.Println("events processed:", pool.processed)
	fmt.Println("sum of event ids:", pool.sum)
	fmt.Println("most threads waiting on the handle set at once:", pool.mostWaiting)
}
```

```output
events processed: 100
sum of event ids: 4950
most threads waiting on the handle set at once: 1
```

- Verdict: stdlib. A `sync.Cond` is the synchronizer. A channel can have many receivers, so the pool, not the channel, keeps the other goroutines off it.
