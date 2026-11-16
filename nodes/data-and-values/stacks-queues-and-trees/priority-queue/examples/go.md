```go run id=pq
package main

import (
	"container/heap"
	"fmt"
)

type task struct {
	priority int
	name     string
}

// taskHeap is a min-heap of tasks, ordered by priority.
type taskHeap []task

func (h taskHeap) Len() int {
	return len(h)
}

func (h taskHeap) Less(i, j int) bool {
	return h[i].priority < h[j].priority
}

func (h taskHeap) Swap(i, j int) {
	h[i], h[j] = h[j], h[i]
}

func (h *taskHeap) Push(x any) {
	*h = append(*h, x.(task))
}

func (h *taskHeap) Pop() any {
	old := *h
	n := len(old)
	item := old[n-1]
	*h = old[:n-1]
	return item
}

// maxInts is a max-heap, because Less uses >.
type maxInts []int

func (h maxInts) Len() int {
	return len(h)
}

func (h maxInts) Less(i, j int) bool {
	return h[i] > h[j]
}

func (h maxInts) Swap(i, j int) {
	h[i], h[j] = h[j], h[i]
}

func (h *maxInts) Push(x any) {
	*h = append(*h, x.(int))
}

func (h *maxInts) Pop() any {
	old := *h
	n := len(old)
	item := old[n-1]
	*h = old[:n-1]
	return item
}

func main() {
	tasks := &taskHeap{}
	input := []task{{3, "update docs"}, {1, "fix outage"}, {2, "review patch"}, {4, "plan sprint"}}
	for _, t := range input {
		heap.Push(tasks, t)
	}

	fmt.Println("peek:", (*tasks)[0].name)
	for tasks.Len() > 0 {
		t := heap.Pop(tasks).(task)
		fmt.Println(t.priority, t.name)
	}

	numbers := &maxInts{3, 1, 2, 4}
	heap.Init(numbers)
	fmt.Print("largest first:")
	for numbers.Len() > 0 {
		fmt.Print(" ", heap.Pop(numbers))
	}
	fmt.Println()
}
```

```output for=pq
peek: fix outage
1 fix outage
2 review patch
3 update docs
4 plan sprint
largest first: 4 3 2 1
```

- Verdict: stdlib. `container/heap` works on any type with `Len`, `Less`, `Swap`, `Push` and `Pop`. The program calls `heap.Push` and `heap.Pop`, not the methods directly.
- `Less` decides the order. With `>`, the heap gives the largest item first.
