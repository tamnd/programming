```go run id=channel
package main

import (
	"fmt"
	"time"
)

func async[T any](work func() T) <-chan T {
	result := make(chan T, 1)
	go func() {
		result <- work()
	}()
	return result
}

func main() {
	price := async(func() int {
		time.Sleep(10 * time.Millisecond)
		return 40
	})
	tax := async(func() int { return 2 })
	fmt.Println(<-price + <-tax)
}
```

```output for=channel
42
```

- Verdict: idiom. Go has no future type. A goroutine that sends its result on a channel with a buffer of one is the usual replacement. The buffer lets the goroutine finish even if no one reads the result.
- A receive from the channel waits for the value. To wait for many goroutines and get the first error, the `golang.org/x/sync/errgroup` package is common.
