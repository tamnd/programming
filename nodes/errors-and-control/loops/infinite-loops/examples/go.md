```go run id=for-with-no-condition-and-a-channel
package main

import "fmt"

// No return after the loop: a for with no condition and no break is a terminating statement.
func firstOver(limit int) int {
	n := 1
	for {
		n *= 3
		if n > limit {
			return n
		}
	}
}

func main() {
	fmt.Println("first power of 3 over 100:", firstOver(100))

	// Read from a channel until it is closed.
	ch := make(chan int)
	go func() {
		for i := 1; i <= 3; i++ {
			ch <- i * 10
		}
		close(ch)
	}()
	for {
		v, ok := <-ch
		if !ok {
			fmt.Println("channel closed")
			break
		}
		fmt.Println("got", v)
	}
}
```

```output for=for-with-no-condition-and-a-channel
first power of 3 over 100: 243
got 10
got 20
got 30
channel closed
```

- Verdict: native. `for { }` with no condition is the infinite loop of Go. `firstOver` needs no `return` after the loop, because the loop has no `break` and so is a terminating statement. The second loop waits on the channel and leaves with `break` when the channel is closed. `for v := range ch` is the shorter form of the same loop.
