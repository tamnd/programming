```go run id=shared-connection
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
)

type connection struct {
	users  atomic.Int32
	closed atomic.Bool
}

func open() *connection {
	c := &connection{}
	c.users.Store(1)
	return c
}

func (c *connection) retain() *connection {
	c.users.Add(1)
	return c
}

func (c *connection) release() {
	if c.users.Add(-1) == 0 {
		c.closed.Store(true)
		fmt.Println("connection closed")
	}
}

func main() {
	conn := open()
	var wg sync.WaitGroup
	for range 4 {
		mine := conn.retain()
		wg.Go(mine.release)
	}
	wg.Wait()
	fmt.Println("closed after workers:", conn.closed.Load())
	conn.release()
	fmt.Println("closed after last release:", conn.closed.Load())
}
```

```output
closed after workers: false
connection closed
closed after last release: true
```

- Verdict: idiom. Go frees memory with a tracing collector and has no reference counts for objects. A program can count the users of a resource by hand.
- `atomic.Int32.Add` returns the value after the change, so exactly one goroutine sees zero and closes the connection.
- The count goes up in `main` before each goroutine starts, so a quick goroutine cannot bring it to zero early.
- For one owner, `defer conn.Close()` is the usual form.
