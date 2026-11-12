```go run id=producer-consumer
package main

import (
	"fmt"
	"sort"
	"sync"
)

func main() {
	buffer := make(chan int, 4) // a send waits when 4 items wait
	var mu sync.Mutex
	var consumed []int

	var producers sync.WaitGroup
	for _, start := range []int{0, 500} {
		producers.Add(1)
		go func(start int) {
			defer producers.Done()
			for item := start; item < start+500; item++ {
				buffer <- item
			}
		}(start)
	}
	var consumers sync.WaitGroup
	for i := 0; i < 2; i++ {
		consumers.Add(1)
		go func() {
			defer consumers.Done()
			for item := range buffer { // the loop ends when the channel is closed and empty
				mu.Lock()
				consumed = append(consumed, item)
				mu.Unlock()
			}
		}()
	}
	producers.Wait()
	close(buffer) // the end of the stream for every consumer
	consumers.Wait()

	sort.Ints(consumed)
	sum, once := 0, len(consumed) == 1000
	for i, x := range consumed {
		sum += x
		once = once && x == i
	}
	fmt.Println("items consumed:", len(consumed))
	fmt.Println("sum of items:", sum)
	if once {
		fmt.Println("each item consumed once: True")
	} else {
		fmt.Println("each item consumed once: False")
	}
}
```

```output
items consumed: 1000
sum of items: 499500
each item consumed once: True
```

- Verdict: native. A channel with a buffer of 4 is the bounded buffer, and `close` ends the `range` loop of each consumer.
