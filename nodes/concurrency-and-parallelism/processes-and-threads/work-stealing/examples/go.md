```go run id=work-stealing-model
package main

import (
	"fmt"
	"strings"
)

// A task is a range [lo, hi). A range of more than 2 numbers splits into two halves;
// a smaller range is a leaf that adds its numbers.
type task struct{ lo, hi int }

// A step-by-step model of a work-stealing scheduler with two workers.
// Each worker has a deque (a slice). Index 0 is the top (oldest task); the end is the bottom (newest task).
func main() {
	const workers = 2
	deques := make([][]task, workers)
	leaves := make([]int, workers)
	total := 0

	deques[0] = append(deques[0], task{0, 8})
	busy := func() bool {
		for _, d := range deques {
			if len(d) > 0 {
				return true
			}
		}
		return false
	}
	for round := 1; busy(); round++ {
		for w := range workers {
			var t task
			how, source := "pops", ""
			if n := len(deques[w]); n > 0 {
				t, deques[w] = deques[w][n-1], deques[w][:n-1] // the owner takes the newest task (LIFO)
			} else {
				victim := -1
				for v := range workers {
					if v != w && len(deques[v]) > 0 {
						victim = v
						break
					}
				}
				if victim == -1 {
					fmt.Printf("round %d: w%d is idle\n", round, w)
					continue
				}
				t, deques[victim] = deques[victim][0], deques[victim][1:] // a thief takes the oldest task (FIFO)
				how, source = "steals", fmt.Sprintf(" from w%d", victim)
			}
			if t.hi-t.lo > 2 {
				mid := (t.lo + t.hi) / 2
				deques[w] = append(deques[w], task{t.lo, mid}, task{mid, t.hi})
				fmt.Printf("round %d: w%d %s [%d, %d)%s and splits it\n", round, w, how, t.lo, t.hi, source)
			} else {
				for i := t.lo; i < t.hi; i++ {
					total += i
				}
				leaves[w]++
				fmt.Printf("round %d: w%d %s [%d, %d)%s and runs it\n", round, w, how, t.lo, t.hi, source)
			}
		}
	}

	parts := make([]string, workers)
	for w, n := range leaves {
		parts[w] = fmt.Sprintf("w%d %d", w, n)
	}
	fmt.Println("leaves run:", strings.Join(parts, ", "))
	fmt.Println("sum:", total)
}
```

```output for=work-stealing-model
round 1: w0 pops [0, 8) and splits it
round 1: w1 steals [0, 4) from w0 and splits it
round 2: w0 pops [4, 8) and splits it
round 2: w1 pops [2, 4) and runs it
round 3: w0 pops [6, 8) and runs it
round 3: w1 pops [0, 2) and runs it
round 4: w0 pops [4, 6) and runs it
round 4: w1 is idle
leaves run: w0 2, w1 2
sum: 28
```

- Verdict: native. The Go runtime scheduler is a work-stealing scheduler. Each P, a processor in the runtime, has a local run queue of goroutines, and an idle P steals half of the goroutines of another P.
- Go code does not see the deques, so this file is a model of the rules, with two slices in place of two workers.
- The owner takes the last element of its slice, and a thief takes the first element of another slice.
