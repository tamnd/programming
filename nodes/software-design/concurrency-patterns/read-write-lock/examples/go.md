```go run id=read-write-lock
package main

import (
	"fmt"
	"runtime"
	"sync"
	"sync/atomic"
	"time"
)

var (
	rw     sync.RWMutex
	pair   [2]int // the writer keeps the two values equal
	inside atomic.Int64
	reads  atomic.Int64
	mixed  atomic.Int64
)

// Part 1: two readers hold the read lock at the same time.
func meet(atMeeting *atomic.Int64, met *atomic.Int64, wg *sync.WaitGroup) {
	defer wg.Done()
	rw.RLock()
	defer rw.RUnlock()
	atMeeting.Add(1)
	deadline := time.Now().Add(5 * time.Second)
	for atMeeting.Load() < 2 && time.Now().Before(deadline) {
		runtime.Gosched()
	}
	if atMeeting.Load() == 2 {
		met.Add(1)
	}
}

// Part 2: readers and a writer share the pair.
func reader(wg *sync.WaitGroup) {
	defer wg.Done()
	for n := 0; n < 1000; n++ {
		rw.RLock()
		inside.Add(1)
		x := pair[0]
		runtime.Gosched() // give the writer a chance to run here
		y := pair[1]
		inside.Add(-1)
		reads.Add(1)
		if x != y {
			mixed.Add(1)
		}
		rw.RUnlock()
	}
}

func writer(writes, overlap *int, wg *sync.WaitGroup) {
	defer wg.Done()
	for i := 1; i <= 100; i++ {
		rw.Lock()
		if inside.Load() > 0 {
			*overlap++
		}
		pair[0] = i
		runtime.Gosched()
		pair[1] = i
		*writes++
		rw.Unlock()
	}
}

func main() {
	var atMeeting, met atomic.Int64
	var wg sync.WaitGroup
	wg.Add(2)
	go meet(&atMeeting, &met, &wg)
	go meet(&atMeeting, &met, &wg)
	wg.Wait()

	writes, overlap := 0, 0
	wg.Add(5)
	for r := 0; r < 4; r++ {
		go reader(&wg)
	}
	go writer(&writes, &overlap, &wg)
	wg.Wait()

	fmt.Println("two readers inside at once:", map[bool]string{true: "True", false: "False"}[met.Load() == 2])
	fmt.Println("reads:", reads.Load())
	fmt.Println("reads that saw a mixed pair:", mixed.Load())
	fmt.Println("writes:", writes)
	fmt.Println("writes that ran with a reader inside:", overlap)
}
```

```output
two readers inside at once: True
reads: 4000
reads that saw a mixed pair: 0
writes: 100
writes that ran with a reader inside: 0
```

- Verdict: stdlib. `sync.RWMutex` gives `RLock` for readers and `Lock` for the writer.
