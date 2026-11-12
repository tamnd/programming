```go run id=guarded-suspension
package main

import (
	"fmt"
	"sync"
)

type Tank struct {
	mu                            sync.Mutex
	cond                          *sync.Cond
	level                         int
	lowest, filled, taken, served int
}

func NewTank() *Tank {
	t := &Tank{}
	t.cond = sync.NewCond(&t.mu)
	return t
}

func (t *Tank) Take(amount int) {
	t.mu.Lock()
	defer t.mu.Unlock()
	// Guarded suspension: wait in a loop until the guard is true.
	for t.level < amount {
		t.cond.Wait()
	}
	t.level -= amount
	t.lowest = min(t.lowest, t.level)
	t.taken += amount
	t.served++
}

func (t *Tank) Fill(amount int) {
	t.mu.Lock()
	defer t.mu.Unlock()
	t.level += amount
	t.filled += amount
	t.cond.Broadcast() // each waiting taker checks its guard again
}

func main() {
	tank := NewTank()
	amounts := []int{5, 3, 8}
	var wg sync.WaitGroup
	for _, a := range amounts {
		wg.Add(1)
		go func() {
			defer wg.Done()
			tank.Take(a)
		}()
	}
	for i := 0; i < 10; i++ {
		tank.Fill(2)
	}
	wg.Wait()

	tank.mu.Lock()
	defer tank.mu.Unlock()
	fmt.Println("units filled:", tank.filled)
	fmt.Println("units taken:", tank.taken)
	fmt.Println("takers served:", tank.served)
	fmt.Println("level never below 0:", map[bool]string{true: "True", false: "False"}[tank.lowest >= 0])
	fmt.Println("units left:", tank.level)
}
```

```output
units filled: 20
units taken: 16
takers served: 3
level never below 0: True
units left: 4
```

- Verdict: stdlib. `sync.Cond` with `Wait` in a `for` loop and `Broadcast`.
