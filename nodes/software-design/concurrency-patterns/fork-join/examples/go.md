```go run id=fork-join
package main

import (
	"fmt"
	"sync"
)

const threshold = 10_000 // a range this small is summed directly

var (
	mu      sync.Mutex
	leaves  int
	largest int64
)

// solve returns the sum of lo, lo + 1, ..., hi - 1.
func solve(lo, hi int64) int64 {
	if hi-lo <= threshold {
		mu.Lock()
		leaves++
		largest = max(largest, hi-lo)
		mu.Unlock()
		var s int64
		for i := lo; i < hi; i++ {
			s += i
		}
		return s
	}
	mid := (lo + hi) / 2
	var left int64
	var wg sync.WaitGroup
	wg.Add(1)
	go func() { // fork: a new goroutine solves the left half
		defer wg.Done()
		left = solve(lo, mid)
	}()
	right := solve(mid, hi) // this goroutine solves the right half
	wg.Wait()               // join: wait for the left half
	return left + right
}

func boolText(b bool) string {
	if b {
		return "True"
	}
	return "False"
}

func main() {
	var n int64 = 1_000_000
	total := solve(1, n+1)
	fmt.Println("sum:", total)
	fmt.Println("leaf tasks:", leaves)
	fmt.Println("largest leaf, at most 10000:", boolText(largest <= threshold))
	fmt.Println("matches n(n+1)/2:", boolText(total == n*(n+1)/2))
}
```

```output
sum: 500000500000
leaf tasks: 128
largest leaf, at most 10000: True
matches n(n+1)/2: True
```

- Verdict: idiom. A goroutine solves the left half, and `sync.WaitGroup` joins it.
