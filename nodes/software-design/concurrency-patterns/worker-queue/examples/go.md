```go run id=worker-queue
package main

import (
	"fmt"
	"sync"
)

const (
	tasks   = 10
	workers = 3
	failsOn = 4 // the first worker that takes this task fails before it does the work
)

// WorkQueue hands out tasks and keeps each one until a worker acknowledges it.
type WorkQueue struct {
	mu         sync.Mutex
	cond       *sync.Cond
	ready      []int
	unacked    map[int]int // delivery tag -> task
	pending    int         // tasks put and not yet acknowledged
	tag        int
	deliveries int
	putBack    int
}

func NewWorkQueue() *WorkQueue {
	q := &WorkQueue{unacked: map[int]int{}}
	q.cond = sync.NewCond(&q.mu)
	return q
}

func (q *WorkQueue) Put(task int) {
	q.mu.Lock()
	defer q.mu.Unlock()
	q.ready = append(q.ready, task)
	q.pending++
	q.cond.Broadcast()
}

// Get returns a tag and a task, or ok == false when every task is acknowledged.
func (q *WorkQueue) Get() (tag, task int, ok bool) {
	q.mu.Lock()
	defer q.mu.Unlock()
	for len(q.ready) == 0 && q.pending > 0 {
		q.cond.Wait()
	}
	if len(q.ready) == 0 {
		return 0, 0, false
	}
	q.tag++
	q.deliveries++
	task, q.ready = q.ready[0], q.ready[1:]
	q.unacked[q.tag] = task
	return q.tag, task, true
}

func (q *WorkQueue) Ack(tag int) {
	q.mu.Lock()
	defer q.mu.Unlock()
	delete(q.unacked, tag)
	q.pending--
	q.cond.Broadcast()
}

// WorkerLost puts the task back on the queue, because its worker went away without an ack.
func (q *WorkQueue) WorkerLost(tag int) {
	q.mu.Lock()
	defer q.mu.Unlock()
	q.ready = append(q.ready, q.unacked[tag])
	delete(q.unacked, tag)
	q.putBack++
	q.cond.Broadcast()
}

func main() {
	work := NewWorkQueue()
	for task := 1; task <= tasks; task++ {
		work.Put(task)
	}

	var mu sync.Mutex
	results := map[int]int{}
	failed := false
	var wg sync.WaitGroup
	for i := 0; i < workers; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for {
				tag, task, ok := work.Get()
				if !ok {
					return
				}
				mu.Lock()
				fail := task == failsOn && !failed
				if fail {
					failed = true
				}
				mu.Unlock()
				if fail {
					work.WorkerLost(tag) // this worker stops here
					return
				}
				mu.Lock()
				results[task] = task * task // the work
				mu.Unlock()
				work.Ack(tag) // only after the work is done
			}
		}()
	}
	wg.Wait()

	all := len(results) == tasks
	for task := 1; task <= tasks; task++ {
		if _, found := results[task]; !found {
			all = false
		}
	}
	fmt.Println("tasks put on the queue:", tasks)
	fmt.Println("workers:", workers)
	fmt.Println("deliveries:", work.deliveries)
	fmt.Println("tasks put back after a worker failed:", work.putBack)
	fmt.Println("tasks still not acknowledged:", work.pending)
	if all {
		fmt.Println("every task has a result: True")
	} else {
		fmt.Println("every task has a result: False")
	}
}
```

```output
tasks put on the queue: 10
workers: 3
deliveries: 11
tasks put back after a worker failed: 1
tasks still not acknowledged: 0
every task has a result: True
```

- Verdict: idiom. A `WorkQueue` type on `sync.Mutex` and `sync.Cond`. A plain channel forgets a value when a goroutine receives it, so it cannot put a task back.
