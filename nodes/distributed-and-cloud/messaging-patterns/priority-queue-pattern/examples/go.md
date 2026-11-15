```go run id=prio-dispatch
package main

import (
	"container/heap"
	"fmt"
	"strings"
)

type msg struct {
	level, seq, arrived int
	id, prio            string
}

// queue implements heap.Interface; the arrival number breaks ties.
type queue []msg

func (q queue) Len() int {
	return len(q)
}

func (q queue) Less(i, j int) bool {
	if q[i].level != q[j].level {
		return q[i].level < q[j].level
	}
	return q[i].seq < q[j].seq
}

func (q queue) Swap(i, j int) {
	q[i], q[j] = q[j], q[i]
}

func (q *queue) Push(x any) {
	*q = append(*q, x.(msg))
}

func (q *queue) Pop() any {
	old := *q
	m := old[len(old)-1]
	*q = old[:len(old)-1]
	return m
}

func main() {
	level := map[string]int{"high": 0, "normal": 1, "low": 2}
	arrivals := []struct {
		tick     int
		id, prio string
	}{
		{0, "l1", "low"}, {0, "l2", "low"}, {0, "h1", "high"}, {1, "n1", "normal"},
		{1, "h2", "high"}, {2, "l3", "low"}, {3, "h3", "high"}, {4, "n2", "normal"},
	}
	q := &queue{}
	var order []string
	seq, i := 0, 0
	for t := 0; i < len(arrivals) || q.Len() > 0; t++ {
		for i < len(arrivals) && arrivals[i].tick == t {
			a := arrivals[i]
			heap.Push(q, msg{level[a.prio], seq, t, a.id, a.prio})
			seq++
			i++
		}
		if q.Len() > 0 {
			m := heap.Pop(q).(msg)
			fmt.Printf("tick %d: %s (%s, waited %d)\n", t, m.id, m.prio, t-m.arrived)
			order = append(order, m.id)
		}
	}
	fmt.Println("order: " + strings.Join(order, " "))
	pos := map[string]int{}
	for k, id := range order {
		pos[id] = k
	}
	fmt.Printf("l1 before l2: %t\n", pos["l1"] < pos["l2"])
}
```

```output
tick 0: h1 (high, waited 0)
tick 1: h2 (high, waited 0)
tick 2: n1 (normal, waited 1)
tick 3: h3 (high, waited 0)
tick 4: n2 (normal, waited 0)
tick 5: l1 (low, waited 5)
tick 6: l2 (low, waited 6)
tick 7: l3 (low, waited 5)
order: h1 h2 n1 h3 n2 l1 l2 l3
l1 before l2: true
```

- Verdict: idiom. `container/heap` works on any type that has the methods `Len`, `Less`, `Swap`, `Push` and `Pop`, and here a slice of messages has them.
- `Less` compares the level first and the arrival number second, so the messages of one priority keep their arrival order.
- `heap.Pop` returns an empty interface, so the caller converts the value back to the message type.
