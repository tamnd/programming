```go run id=task-policy
package main

import (
	"fmt"
	"slices"
	"strings"
)

type Task struct {
	Name string
	Time int
}

// Policy decides which task in the queue runs next.
type Policy func(queue []Task) int

// runAll is the mechanism: it runs the tasks; it does not decide the order.
func runAll(tasks []Task, policy Policy) string {
	queue := slices.Clone(tasks)
	var done []string
	for len(queue) > 0 {
		next := policy(queue)
		done = append(done, queue[next].Name)
		queue = slices.Delete(queue, next, next+1)
	}
	return strings.Join(done, " ")
}

func firstCome([]Task) int { return 0 }

func shortestFirst(queue []Task) int {
	best := 0
	for i, t := range queue {
		if t.Time < queue[best].Time {
			best = i
		}
	}
	return best
}

func main() {
	tasks := []Task{{"backup", 30}, {"email", 2}, {"report", 10}}
	fmt.Printf("first come: %s\n", runAll(tasks, firstCome))
	fmt.Printf("shortest first: %s\n", runAll(tasks, shortestFirst))
}
```

```output
first come: backup email report
shortest first: email report backup
```
