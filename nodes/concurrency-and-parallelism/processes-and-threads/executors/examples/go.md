```go run id=executor-policies
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

// An Executor runs a task. This program uses three implementations.
type Executor func(task func())

func direct(task func()) { task() }

func goroutinePerTask(task func()) { go task() }

// pool returns an executor with a fixed number of workers, and a function that stops them.
func pool(workers int) (Executor, func()) {
	tasks := make(chan func())
	var wg sync.WaitGroup
	for range workers {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for task := range tasks {
				task()
			}
		}()
	}
	execute := func(task func()) { tasks <- task }
	stop := func() { close(tasks); wg.Wait() }
	return execute, stop
}

// mostAtOnce runs count tasks with the executor and returns the most that ran at the same time.
func mostAtOnce(execute Executor, count int) int32 {
	var running, most atomic.Int32
	var done sync.WaitGroup
	done.Add(count)
	for range count {
		execute(func() {
			defer done.Done()
			now := running.Add(1)
			for {
				seen := most.Load()
				if now <= seen || most.CompareAndSwap(seen, now) {
					break
				}
			}
			time.Sleep(20 * time.Millisecond)
			running.Add(-1)
		})
	}
	done.Wait()
	return most.Load()
}

func main() {
	fmt.Println("direct: most at once:", mostAtOnce(direct, 4))
	fmt.Println("goroutine per task: most at once:", mostAtOnce(goroutinePerTask, 4))
	execute, stop := pool(2)
	fmt.Println("pool of 2: most at once:", mostAtOnce(execute, 4))
	stop()
}
```

```output for=executor-policies
direct: most at once: 1
goroutine per task: most at once: 4
pool of 2: most at once: 2
```

- Verdict: absent. Go has no executor type, because the `go` statement and the runtime scheduler do this work. A program that needs another policy, such as a limit on the number of workers, writes a small executor with a channel.
- `Executor` here is a function type. `mostAtOnce` uses only that type, so the same code runs with each policy.
- The pool limits the work to two tasks at the same time. A goroutine for each task has no limit.
