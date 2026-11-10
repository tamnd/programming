```go run id=pipeline
package main

import "fmt"

func produce(id int, out chan<- int, done chan<- bool) {
	for i := 1; i <= 3; i++ {
		out <- id * 10 * i
	}
	done <- true
}

func main() {
	jobs := make(chan int, 2) // buffered: capacity 2
	done := make(chan bool)   // unbuffered: a rendezvous
	for id := 1; id <= 2; id++ {
		go produce(id, jobs, done)
	}
	go func() {
		<-done
		<-done
		close(jobs) // both producers have finished
	}()
	total := 0
	for v := range jobs { // ends when jobs is closed and empty
		total += v
	}
	v, ok := <-jobs
	fmt.Println("total:", total)
	fmt.Println("after close:", v, ok)
}
```

```output for=pipeline
total: 180
after close: 0 false
```

```go run id=select
package main

import (
	"fmt"
	"time"
)

func main() {
	ch := make(chan string, 1) // room for the late value, so the sender never blocks
	select {
	case msg := <-ch:
		fmt.Println("got", msg)
	default:
		fmt.Println("no value ready")
	}
	go func() {
		time.Sleep(500 * time.Millisecond)
		ch <- "late"
	}()
	select {
	case msg := <-ch:
		fmt.Println("got", msg)
	case <-time.After(50 * time.Millisecond):
		fmt.Println("timeout")
	}
}
```

```output for=select
no value ready
timeout
```

- Verdict: native. `chan T`, the `<-` operator, `close`, `select` and `range` are part of the language.
- `chan<- int` is a send-only type. The compiler rejects a receive on it.
- A receive after `close` returns at once with the zero value and `ok == false`.
