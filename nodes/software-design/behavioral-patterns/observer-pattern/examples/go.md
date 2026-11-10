```go run id=sensor
package main

import (
	"fmt"
	"sync"
)

type Sensor struct {
	mu          sync.Mutex
	subscribers map[int]chan float64
	next        int
}

// Subscribe returns a channel of readings and a function that ends the subscription.
func (s *Sensor) Subscribe() (<-chan float64, func()) {
	s.mu.Lock()
	defer s.mu.Unlock()
	if s.subscribers == nil {
		s.subscribers = map[int]chan float64{}
	}
	id := s.next
	s.next++
	ch := make(chan float64, 8)
	s.subscribers[id] = ch
	return ch, func() {
		s.mu.Lock()
		defer s.mu.Unlock()
		delete(s.subscribers, id)
		close(ch)
	}
}

func (s *Sensor) Set(value float64) {
	s.mu.Lock()
	defer s.mu.Unlock()
	for _, ch := range s.subscribers {
		ch <- value
	}
}

func main() {
	var sensor Sensor
	var wg sync.WaitGroup
	display, stopDisplay := sensor.Subscribe()
	alarm, stopAlarm := sensor.Subscribe()
	wg.Add(2)
	go func() {
		defer wg.Done()
		for value := range display {
			fmt.Printf("display: %.1f\n", value)
		}
	}()
	go func() {
		defer wg.Done()
		for value := range alarm {
			if value > 30 {
				fmt.Printf("alarm: %.1f is above 30\n", value)
			}
		}
	}()
	sensor.Set(21.5)
	sensor.Set(30.5)
	stopDisplay()
	sensor.Set(31.0)
	stopAlarm()
	wg.Wait()
}
```

```output for=sensor match=unordered
alarm: 30.5 is above 30
alarm: 31.0 is above 30
display: 21.5
display: 30.5
```

- Verdict: idiom. Go has no observer type in the standard library. A channel for each subscriber is the usual form, and a slice of `func(float64)` is the usual synchronous form.
- Each subscriber reads its channel on its own goroutine. The two goroutines run at the same time, so the lines of the display and the lines of the alarm can interleave in any order. The output block compares the lines as a set.
- The channel has a buffer of 8. When a buffer is full, `Set` blocks while it holds the lock, so a slow subscriber slows the subject. A subject that must not block drops the value or uses a `select` with a `default` case.
- The function from `Subscribe` closes the channel, so the `for range` loop of the subscriber ends. The `sync.WaitGroup` lets `main` wait until both loops have ended.
