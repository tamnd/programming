```go run id=master-worker-pattern
package main

import (
	"fmt"
	"strings"
	"sync"
)

const (
	limit   = 20000
	workers = 4
)

// countPrimes is the work of one worker: it counts the primes in [lo, hi).
func countPrimes(lo, hi int) int {
	count := 0
	if lo < 2 {
		lo = 2
	}
	for n := lo; n < hi; n++ {
		d := 2
		for d*d <= n && n%d != 0 {
			d++
		}
		if d*d > n {
			count++
		}
	}
	return count
}

func main() {
	// 1. Split the work into one part for each worker.
	parts := make([][2]int, workers)
	for i := range parts {
		parts[i] = [2]int{limit * i / workers, limit * (i + 1) / workers}
	}
	// 2. Start the workers, then wait for all of them.
	results := make([]int, workers) // each worker writes only its own slot
	var wg sync.WaitGroup
	for i, p := range parts {
		wg.Add(1)
		go func(i int, p [2]int) {
			defer wg.Done()
			results[i] = countPrimes(p[0], p[1])
		}(i, p)
	}
	wg.Wait()
	// 3. Combine the partial results.
	total := 0
	for _, r := range results {
		total += r
	}

	shown := make([]string, len(parts))
	for i, p := range parts {
		shown[i] = fmt.Sprintf("(%d, %d)", p[0], p[1])
	}
	nums := make([]string, len(results))
	for i, r := range results {
		nums[i] = fmt.Sprint(r)
	}
	fmt.Println("parts: [" + strings.Join(shown, ", ") + "]")
	fmt.Println("primes in each part: [" + strings.Join(nums, ", ") + "]")
	fmt.Printf("primes below %d from the master: %d\n", limit, total)
	if total == countPrimes(0, limit) {
		fmt.Println("same as one loop: True")
	} else {
		fmt.Println("same as one loop: False")
	}
}
```

```output
parts: [(0, 5000), (5000, 10000), (10000, 15000), (15000, 20000)]
primes in each part: [669, 560, 525, 508]
primes below 20000 from the master: 2262
same as one loop: True
```

- Verdict: native. A goroutine for each part writes its own slot, and a `sync.WaitGroup` makes the master wait.
