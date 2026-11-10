```go run id=sums
package main

import (
	"fmt"
	"sync"
)

func main() {
	results := make([]int, 4)
	var wg sync.WaitGroup
	for i := range 4 {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for n := i * 250; n < (i+1)*250; n++ {
				results[i] += n
			}
		}()
	}
	wg.Wait()
	fmt.Println(results)

	var many sync.WaitGroup
	count := make([]int, 10000)
	for i := range count {
		many.Add(1)
		go func() {
			defer many.Done()
			count[i] = 1
		}()
	}
	many.Wait()
	total := 0
	for _, c := range count {
		total += c
	}
	fmt.Println("goroutines run:", total)
}
```

```output for=sums
[31125 93625 156125 218625]
goroutines run: 10000
```

- Verdict: native. The `go` statement starts a goroutine. The runtime multiplexes goroutines onto a set of OS threads (M:N).
- A goroutine has no handle and no `join`. The result of the function is discarded. `sync.WaitGroup` waits for a group, and each goroutine writes to its own slice element.
- Starting 10,000 goroutines is a normal thing to do, because each one begins with a small stack that grows when needed.
- When `main` returns, the program ends and does not wait for other goroutines (see Pitfalls).
