```go run id=no-handle
package main

import "fmt"

func main() {
	done := make(chan int)
	go func() {
		done <- 6 * 7
	}()
	fmt.Println("result through a channel:", <-done)

	block := make(chan struct{})
	go func() {
		<-block
		fmt.Println("never printed")
	}()
	fmt.Println("main returns; the blocked goroutine ends with the program")
}
```

```output for=no-handle
result through a channel: 42
main returns; the blocked goroutine ends with the program
```

- Verdict: idiom. A `go` statement gives no handle, so Go has no join and no detach. The goroutine itself reports its end, here by a send on a channel.
- The receive `<-done` waits until the first goroutine has sent its result, so it acts as a join that also carries a value.
- The second goroutine waits on a channel that nobody closes. When `main` returns, the program exits and the goroutine ends with it.
- To wait for many goroutines, use a `sync.WaitGroup`, as in [[wait-for-threads-to-finish]].
