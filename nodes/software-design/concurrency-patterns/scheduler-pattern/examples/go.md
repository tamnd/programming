```go run id=scheduler-pattern
package main

import (
	"fmt"
	"strings"
	"sync"
	"sync/atomic"
	"time"
)

type entry struct {
	priority, arrival int
	name              string
}

// Scheduler decides which waiting goroutine uses the printer next.
type Scheduler struct {
	mu       sync.Mutex
	cond     *sync.Cond
	busy     bool
	waiting  []entry
	arrivals int
	chosen   int // the arrival number of the chosen entry
}

func NewScheduler() *Scheduler {
	s := &Scheduler{}
	s.cond = sync.NewCond(&s.mu)
	return s
}

func (s *Scheduler) Enter(name string, priority int) {
	s.mu.Lock()
	defer s.mu.Unlock()
	if !s.busy && len(s.waiting) == 0 {
		s.busy = true
		return
	}
	s.arrivals++
	e := entry{priority, s.arrivals, name}
	s.waiting = append(s.waiting, e)
	s.cond.Broadcast()
	// Sleep until the scheduler chooses this goroutine.
	for s.chosen != e.arrival {
		s.cond.Wait()
	}
	s.chosen = 0
}

func (s *Scheduler) Done() {
	s.mu.Lock()
	defer s.mu.Unlock()
	if len(s.waiting) == 0 {
		s.busy = false
		return
	}
	// The policy: the lowest priority number first, then the first to arrive.
	best := 0
	for i, e := range s.waiting {
		b := s.waiting[best]
		if e.priority < b.priority || (e.priority == b.priority && e.arrival < b.arrival) {
			best = i
		}
	}
	s.chosen = s.waiting[best].arrival // the printer stays busy and passes to that goroutine
	s.waiting = append(s.waiting[:best], s.waiting[best+1:]...)
	s.cond.Broadcast()
}

func (s *Scheduler) WaitUntilWaiting(n int) int {
	deadline := time.Now().Add(5 * time.Second)
	for {
		s.mu.Lock()
		got := len(s.waiting)
		s.mu.Unlock()
		if got >= n || time.Now().After(deadline) {
			return got
		}
		time.Sleep(time.Millisecond)
	}
}

var (
	scheduler     = NewScheduler()
	orderMu       sync.Mutex
	order         []string
	onPrinter     atomic.Int32
	mostOnPrinter int32
)

func usePrinter(name string) {
	now := onPrinter.Add(1)
	orderMu.Lock()
	mostOnPrinter = max(mostOnPrinter, now)
	order = append(order, name)
	orderMu.Unlock()
	onPrinter.Add(-1)
}

func main() {
	scheduler.Enter("first", 0) // the first job takes the free printer
	usePrinter("first")
	jobs := []struct {
		name     string
		priority int
	}{{"report", 2}, {"invoice", 1}, {"photo", 3}, {"label", 1}}
	var wg sync.WaitGroup
	for i, job := range jobs {
		wg.Add(1)
		go func() {
			defer wg.Done()
			scheduler.Enter(job.name, job.priority)
			usePrinter(job.name)
			scheduler.Done()
		}()
		scheduler.WaitUntilWaiting(i + 1) // start the jobs in a known order
	}
	fmt.Println("jobs waiting when the first job ends:", scheduler.WaitUntilWaiting(len(jobs)))
	scheduler.Done()
	wg.Wait()

	fmt.Println("print order:", strings.Join(order, ", "))
	fmt.Println("jobs on the printer at once, at most:", mostOnPrinter)
}
```

```output
jobs waiting when the first job ends: 4
print order: first, invoice, label, report, photo
jobs on the printer at once, at most: 1
```

- Verdict: idiom. A type with `sync.Mutex` and `sync.Cond`; `Enter` calls `Wait` in a `for` loop until `Done` chooses its arrival number.
