```go run id=competing-workers
package main

import (
	"errors"
	"fmt"
	"sort"
	"strings"
	"sync"
	"time"
)

const maxDeliveries = 3
const workers = 3

type delivery struct{ job, attempt int }

type event struct {
	job, attempt int
	text         string
}

func handle(job, attempt int) (int, error) {
	time.Sleep(40 * time.Millisecond)
	if job == 4 && attempt == 1 {
		return 0, errors.New("timeout of the database")
	}
	if job == 7 {
		return 0, errors.New("bad payload")
	}
	return job * job, nil
}

func main() {
	jobs := make(chan delivery, 16)
	var mu sync.Mutex
	results := map[int]int{}
	var events []event
	pending, deliveries := 8, 0

	for job := 1; job <= 8; job++ {
		jobs <- delivery{job, 1}
	}
	var wg sync.WaitGroup
	for w := 0; w < workers; w++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for d := range jobs {
				mu.Lock()
				deliveries++
				mu.Unlock()
				result, err := handle(d.job, d.attempt)
				mu.Lock()
				if err != nil && d.attempt < maxDeliveries {
					events = append(events, event{d.job, d.attempt,
						fmt.Sprintf("failed on delivery %d (%v), back on the queue", d.attempt, err)})
					jobs <- delivery{d.job, d.attempt + 1}
					mu.Unlock()
					continue
				}
				if err != nil {
					events = append(events, event{d.job, d.attempt,
						fmt.Sprintf("failed on delivery %d (%v), to the dead letter channel", d.attempt, err)})
				} else {
					results[d.job] = result
				}
				pending--
				if pending == 0 {
					close(jobs)
				}
				mu.Unlock()
			}
		}()
	}
	wg.Wait()

	fmt.Printf("%d workers, 8 jobs on one queue\n", workers)
	sort.Slice(events, func(i, j int) bool {
		if events[i].job != events[j].job {
			return events[i].job < events[j].job
		}
		return events[i].attempt < events[j].attempt
	})
	for _, e := range events {
		fmt.Printf("job %d: %s\n", e.job, e.text)
	}
	keys := make([]int, 0, len(results))
	for k := range results {
		keys = append(keys, k)
	}
	sort.Ints(keys)
	parts := make([]string, len(keys))
	for i, k := range keys {
		parts[i] = fmt.Sprint(results[k])
	}
	fmt.Printf("done: %d jobs, results %s\n", len(results), strings.Join(parts, " "))
	fmt.Printf("deliveries: %d\n", deliveries)
}
```

```output
3 workers, 8 jobs on one queue
job 4: failed on delivery 1 (timeout of the database), back on the queue
job 7: failed on delivery 1 (bad payload), back on the queue
job 7: failed on delivery 2 (bad payload), back on the queue
job 7: failed on delivery 3 (bad payload), to the dead letter channel
done: 7 jobs, results 1 4 9 16 25 36 64
deliveries: 11
```

- Verdict: idiom. Three goroutines that `range` over one channel are competing consumers; the Go runtime gives each value to one receiver.
- A failed job is sent back on the channel; the buffer of 16 keeps the send from blocking while the worker holds the mutex.
- The last finished job closes the channel, which ends the `range` loop of each goroutine.
