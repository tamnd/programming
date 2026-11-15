```go run id=competing-workers
package main

import (
	"fmt"
	"slices"
	"strings"
	"sync"
)

type result struct{ job, worker string }

func main() {
	jobs := make(chan string)
	done := make(chan result, 9)
	var wg sync.WaitGroup
	for i := 1; i <= 3; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for job := range jobs { // each value is received by one goroutine
				done <- result{job, fmt.Sprintf("w%d", i)}
			}
		}()
	}
	for n := 1; n <= 9; n++ {
		jobs <- fmt.Sprintf("job-%d", n)
	}
	close(jobs)
	wg.Wait()
	close(done)

	var names []string
	seen := map[string]bool{}
	for r := range done {
		names = append(names, r.job)
		seen[r.job] = true
	}
	slices.Sort(names)
	fmt.Println("handled:", strings.Join(names, " "))
	fmt.Println("each job once:", len(names) == 9 && len(seen) == 9)
}
```

```output
handled: job-1 job-2 job-3 job-4 job-5 job-6 job-7 job-8 job-9
each job once: true
```

- Verdict: native. A value sent on a Go channel is received by one of the goroutines that read it, so `for job := range jobs` in three goroutines is a point-to-point channel with three competing receivers.
- `jobs` has no buffer, so each send waits for a free worker. This is a simple form of the prefetch limit of a broker: a busy worker gets no new job.
- `close(jobs)` ends the `range` loops. The `done` channel has room for the nine results, so no worker waits on it.
