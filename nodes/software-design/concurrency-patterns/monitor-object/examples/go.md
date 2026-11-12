```go run id=monitor
package main

import (
	"fmt"
	"sync"
)

// MessageQueue is a monitor object: one mutex and two sync.Cond values on it.
type MessageQueue struct {
	mu       sync.Mutex
	notFull  *sync.Cond
	notEmpty *sync.Cond
	items    []int
	capacity int
	puts     int
	gets     int
	largest  int
}

func NewMessageQueue(capacity int) *MessageQueue {
	q := &MessageQueue{capacity: capacity}
	q.notFull = sync.NewCond(&q.mu)
	q.notEmpty = sync.NewCond(&q.mu)
	return q
}

func (q *MessageQueue) Put(message int) {
	q.mu.Lock()
	defer q.mu.Unlock()
	for len(q.items) == q.capacity {
		q.notFull.Wait() // unlocks q.mu while it waits
	}
	q.items = append(q.items, message)
	q.puts++
	q.largest = max(q.largest, len(q.items))
	q.notEmpty.Signal()
}

func (q *MessageQueue) Get() int {
	q.mu.Lock()
	defer q.mu.Unlock()
	for len(q.items) == 0 {
		q.notEmpty.Wait()
	}
	message := q.items[0]
	q.items = q.items[1:]
	q.gets++
	q.notFull.Signal()
	return message
}

func main() {
	q := NewMessageQueue(2)
	var sums [2]int
	var wg sync.WaitGroup
	for p := 0; p < 4; p++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for i := 0; i < 250; i++ {
				q.Put(p*250 + i)
			}
		}()
	}
	for c := 0; c < 2; c++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for i := 0; i < 500; i++ {
				sums[c] += q.Get()
			}
		}()
	}
	wg.Wait()
	fmt.Println("messages put:", q.puts)
	fmt.Println("messages got:", q.gets)
	fmt.Println("sum of messages got:", sums[0]+sums[1])
	verdict := "False"
	if q.largest <= 2 {
		verdict = "True"
	}
	fmt.Println("queue never held more than 2:", verdict)
}
```

```output
messages put: 1000
messages got: 1000
sum of messages got: 499500
queue never held more than 2: True
```

- Verdict: stdlib. `sync.Cond` takes the `Locker` that must be held when the code calls `Wait`, so both conditions share `q.mu`.
