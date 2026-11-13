```go run id=event-loop-pattern
package main

import (
	"container/heap"
	"fmt"
)

type timer struct {
	due, order int
	callback   func()
}

// timerHeap keeps the timers in order of due time, then of order.
type timerHeap []timer

func (h timerHeap) Len() int { return len(h) }
func (h timerHeap) Less(i, j int) bool {
	return h[i].due < h[j].due || (h[i].due == h[j].due && h[i].order < h[j].order)
}
func (h timerHeap) Swap(i, j int) { h[i], h[j] = h[j], h[i] }
func (h *timerHeap) Push(x any)   { *h = append(*h, x.(timer)) }
func (h *timerHeap) Pop() any {
	old := *h
	t := old[len(old)-1]
	*h = old[:len(old)-1]
	return t
}

// EventLoop takes ready callbacks from a queue on one goroutine and runs each one to the end.
type EventLoop struct {
	ready  []func() // callbacks to run in this turn or the next
	timers timerHeap
	now    int // a counter clock, so the output does not depend on the machine
	order  int
	turns  int
}

func (l *EventLoop) CallSoon(callback func()) { l.ready = append(l.ready, callback) }

func (l *EventLoop) CallLater(delay int, callback func()) {
	l.order++
	heap.Push(&l.timers, timer{l.now + delay, l.order, callback})
}

func (l *EventLoop) Run() {
	for len(l.ready) > 0 || len(l.timers) > 0 {
		if len(l.ready) == 0 {
			l.now = l.timers[0].due // a real loop sleeps or waits for I/O here
		}
		for len(l.timers) > 0 && l.timers[0].due <= l.now {
			l.ready = append(l.ready, heap.Pop(&l.timers).(timer).callback)
		}
		l.turns++
		batch := l.ready // callbacks added now wait for the next turn
		l.ready = nil
		for _, callback := range batch {
			callback()
		}
	}
}

func main() {
	loop := &EventLoop{}
	var log []string
	say := func(text string) func() {
		return func() { log = append(log, fmt.Sprintf("time %d: %s", loop.now, text)) }
	}

	loop.CallLater(20, say("timer of 20"))
	loop.CallLater(10, say("timer of 10"))
	loop.CallSoon(func() {
		say("first callback")()
		loop.CallSoon(say("callback that the first callback added"))
	})
	loop.CallSoon(say("second callback"))
	loop.Run()

	fmt.Println("order of the callbacks:")
	for _, line := range log {
		fmt.Println(" ", line)
	}
	fmt.Println("turns of the loop:", loop.turns)
}
```

```output
order of the callbacks:
  time 0: first callback
  time 0: second callback
  time 0: callback that the first callback added
  time 10: timer of 10
  time 20: timer of 20
turns of the loop: 4
```

- Verdict: idiom. A small loop type with a slice of callbacks and a `container/heap` of timers. All callbacks run on the goroutine that calls `Run`.
