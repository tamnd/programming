```go run id=jobs
package main

import (
	"fmt"
	"runtime"
)

func runJob(job int, items []int) (result int, err error) {
	defer fmt.Println("cleanup job", job)
	defer func() {
		if r := recover(); r != nil {
			_, isRuntime := r.(runtime.Error)
			err = fmt.Errorf("recovered (runtime error: %v): %v", isRuntime, r)
		}
	}()
	return items[job] * 10, nil
}

func main() {
	items := []int{1, 2}
	for _, job := range []int{0, 5, 1} {
		result, err := runJob(job, items)
		if err != nil {
			fmt.Println("job", job, "failed:", err)
			continue
		}
		fmt.Println("job", job, "->", result)
	}
}
```

```output
cleanup job 0
job 0 -> 10
cleanup job 5
job 5 failed: recovered (runtime error: true): runtime error: index out of range [5] with length 2
cleanup job 1
job 1 -> 20
```

- Verdict: native. `panic` stops the function and runs its deferred calls, then those of each caller. A run-time error such as an index out of range is a panic with a value of type `runtime.Error`.
- `recover` stops the panic only when a deferred function calls it directly. The deferred function can then set the named result `err`, so the panic becomes an ordinary error value at the boundary.
- Deferred calls run in reverse order. The recovering function was deferred last, so it runs first, and the line `cleanup job 5` comes after the recovery.

A panic that no function recovers ends the whole program, even when it starts in another goroutine. `recover` in `main` does not help, because it works only in the goroutine that panicked. The program prints the panic value and a stack trace to standard error, and it exits with status 2.

```go fails expect=exit:2 id=goroutine
package main

import (
	"fmt"
	"sync"
)

func main() {
	defer func() {
		fmt.Println("recovered in main:", recover())
	}()
	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		defer wg.Done()
		var items []int
		fmt.Println(items[3])
	}()
	wg.Wait()
}
```

`recover` returns `nil` when it is not called directly by a deferred function. Here the deferred function calls a helper, and the helper calls `recover`, so the panic goes on.

```go fails expect=panic id=indirect
package main

import "fmt"

func logPanic() {
	if r := recover(); r != nil {
		fmt.Println("recovered:", r)
	}
}

func main() {
	defer func() {
		logPanic()
	}()
	panic("broken invariant")
}
```

The fix is `defer logPanic()`, which makes `logPanic` itself the deferred function.
