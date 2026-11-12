```go run id=future-pattern
package main

import (
	"fmt"
	"strings"
	"sync"
)

var prices = map[string]int{"apple": 12, "bread": 7, "cheese": 30}

// lookup returns a future at once: a channel that receives the one result.
func lookup(item string, arrived *sync.WaitGroup, release <-chan struct{}) <-chan int {
	future := make(chan int, 1)
	go func() {
		arrived.Done()
		<-release // wait until all 3 lookups and the caller are here
		future <- prices[item]
	}()
	return future
}

func ready(future <-chan int) bool {
	return len(future) > 0
}

func main() {
	items := []string{"apple", "bread", "cheese"}
	// The lookups wait for the caller, so the output does not depend on timing.
	var arrived sync.WaitGroup
	arrived.Add(len(items))
	release := make(chan struct{})

	var futures []<-chan int
	for _, item := range items {
		futures = append(futures, lookup(item, &arrived, release))
	}
	fmt.Println("lookups started:", len(futures))
	anyDone := false
	for _, f := range futures {
		anyDone = anyDone || ready(f)
	}
	fmt.Println("done right after the calls:", map[bool]string{true: "True", false: "False"}[anyDone])
	arrived.Wait() // the caller is free to do other work here
	close(release)
	fmt.Println("all three lookups ran at once: True")
	var results []string
	total := 0
	for _, f := range futures {
		p := <-f // wait for each result
		results = append(results, fmt.Sprint(p))
		total += p
	}

	fmt.Println("prices: [" + strings.Join(results, ", ") + "]")
	fmt.Println("total:", total)
}
```

```output
lookups started: 3
done right after the calls: False
all three lookups ran at once: True
prices: [12, 7, 30]
total: 49
```

- Verdict: idiom. A function starts a goroutine and returns a channel with a buffer of one. A receive from the channel waits for the value, and `len` of the channel shows whether the value is there.
