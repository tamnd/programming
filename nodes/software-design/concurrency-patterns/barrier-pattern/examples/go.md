```go run id=barrier-pattern
package main

import (
	"fmt"
	"strings"
	"sync"
)

// Barrier is a cyclic barrier for a fixed number of parties.
type Barrier struct {
	mu         sync.Mutex
	cond       *sync.Cond
	parties    int
	arrived    int
	generation int
	action     func()
}

func NewBarrier(parties int, action func()) *Barrier {
	b := &Barrier{parties: parties, action: action}
	b.cond = sync.NewCond(&b.mu)
	return b
}

func (b *Barrier) Wait() {
	b.mu.Lock()
	defer b.mu.Unlock()
	gen := b.generation
	b.arrived++
	if b.arrived == b.parties {
		b.action() // runs once per trip, before the goroutines are released
		b.arrived = 0
		b.generation++ // a new generation lets the barrier be used again
		b.cond.Broadcast()
		return
	}
	for gen == b.generation {
		b.cond.Wait()
	}
}

const N = 8

func show(v []int) string {
	parts := make([]string, len(v))
	for i, x := range v {
		parts[i] = fmt.Sprint(x)
	}
	return "[" + strings.Join(parts, ", ") + "]"
}

func main() {
	values := make([]int, N)
	for i := range values {
		values[i] = i + 1
	}
	distances := []int{1, 2, 4} // each step adds the value that is d places to the left
	fmt.Println("input:", show(values))
	trips := 0
	barrier := NewBarrier(N, func() { trips++ })

	var wg sync.WaitGroup
	for i := 0; i < N; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for _, d := range distances {
				// Phase 1: read a value that the last step wrote.
				add := 0
				if i >= d {
					add = values[i-d]
				}
				barrier.Wait() // nobody writes until every worker has read
				// Phase 2: write the new value.
				values[i] += add
				barrier.Wait() // nobody reads until every worker has written
			}
		}()
	}
	wg.Wait()

	matches := true
	for i, v := range values {
		matches = matches && v == (i+1)*(i+2)/2
	}
	fmt.Println("steps:", len(distances))
	fmt.Println("barrier trips:", trips)
	fmt.Println("prefix sums:", show(values))
	fmt.Println("matches a running total:", map[bool]string{true: "True", false: "False"}[matches])
}
```

```output
input: [1, 2, 3, 4, 5, 6, 7, 8]
steps: 3
barrier trips: 6
prefix sums: [1, 3, 6, 10, 15, 21, 28, 36]
matches a running total: True
```

- Verdict: idiom. The standard library has no barrier, so the example builds a cyclic one from `sync.Mutex`, `sync.Cond` and a generation counter. `sync.WaitGroup` waits once and is not cyclic.
