```go run id=partial
package main

import (
	"fmt"
	"sync"
	"time"
)

func main() {
	var a, b sync.Mutex
	var ready sync.WaitGroup
	ready.Add(2)
	worker := func(first, second *sync.Mutex) {
		first.Lock()
		ready.Done()
		ready.Wait()
		second.Lock()
	}
	go worker(&a, &b)
	go worker(&b, &a)

	done := make(chan struct{})
	go func() {
		ready.Wait()
		a.Lock()
		close(done)
	}()

	select {
	case <-done:
		fmt.Println("done")
	case <-time.After(200 * time.Millisecond):
		fmt.Println("timeout: the workers wait for each other")
	}
}
```

```output for=partial
timeout: the workers wait for each other
```

- Verdict: native. Goroutines block on `sync.Mutex`, on channels and on `sync.WaitGroup`, and each of these waits can be part of a cycle.
- The runtime stops a program in which no goroutine can run, with `fatal error: all goroutines are asleep - deadlock!` and exit code 2. Here the timer of `time.After` is pending, so the runtime check does not fire, and the `select` reports the deadlock. When `main` returns, the blocked goroutines end with the program.
- The same two workers with `wg.Wait()` in `main` and no timer stop with the runtime error:

```go fails expect=exit:2 id=global
package main

import "sync"

func main() {
	var a, b sync.Mutex
	var ready, done sync.WaitGroup
	ready.Add(2)
	worker := func(first, second *sync.Mutex) {
		defer done.Done()
		first.Lock()
		ready.Done()
		ready.Wait()
		second.Lock()
	}
	done.Add(2)
	go worker(&a, &b)
	go worker(&b, &a)
	done.Wait()
}
```
