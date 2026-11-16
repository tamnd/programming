```go run id=per-thread-values
package main

import (
	"context"
	"fmt"
	"sync"
)

// Go has no thread-local storage. Each goroutine gets its own state as a value that it owns.
type state struct{ counter int }

func (s *state) bump(times int) int {
	for range times {
		s.counter++
	}
	return s.counter
}

type userKey struct{}

// show reads a value that travels with the context, not with the goroutine.
func show(ctx context.Context, text string) {
	user, ok := ctx.Value(userKey{}).(string)
	if !ok {
		user = "none"
	}
	fmt.Println(text, user)
}

func main() {
	results := make([]int, 3)
	var wg sync.WaitGroup
	for i := 1; i <= 2; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			var s state // this goroutine's own state
			results[i] = s.bump(i)
		}()
	}
	wg.Wait()
	var mainState state
	results[0] = mainState.bump(3)
	fmt.Println("main: counter =", results[0])
	fmt.Println("goroutine 1: counter =", results[1])
	fmt.Println("goroutine 2: counter =", results[2])

	// A worker goroutine that runs one task after another. Each task gets its own context,
	// so a value from one task cannot reach the next.
	tasks := make(chan func())
	done := make(chan struct{})
	go func() {
		for task := range tasks {
			task()
		}
		close(done)
	}()
	tasks <- func() {
		show(context.WithValue(context.Background(), userKey{}, "alice"), "the task with the value sees user:")
	}
	tasks <- func() { show(context.Background(), "a later task on the same goroutine sees user:") }
	close(tasks)
	<-done
}
```

```output for=per-thread-values
main: counter = 3
goroutine 1: counter = 1
goroutine 2: counter = 2
the task with the value sees user: alice
a later task on the same goroutine sees user: none
```

- Verdict: absent. Go has no thread-local or goroutine-local storage, and goroutines have no ID that a program could use as a key. The Go FAQ explains that this keeps code free to use more goroutines for one job.
- Each goroutine gets its own state as a value that it owns. Here the closure declares `s` inside the goroutine.
- For values that belong to a request, Go passes a `context.Context` as an argument. A value in a context reaches only the code that gets that context, so the second task does not see it.
