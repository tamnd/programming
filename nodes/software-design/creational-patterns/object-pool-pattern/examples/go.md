```go run id=pool
package main

import (
	"fmt"
	"slices"
	"strings"
	"sync/atomic"
)

type Buffer struct {
	ID   int // a real buffer would be costly to create
	Text strings.Builder
}

// Pool keeps idle items in a buffered channel, the "leaky buffer" of
// Effective Go. The channel is safe for many goroutines.
type Pool struct {
	idle    chan *Buffer
	created atomic.Int64
}

func NewPool(maxIdle int) *Pool {
	return &Pool{idle: make(chan *Buffer, maxIdle)}
}

func (p *Pool) Acquire() *Buffer {
	select {
	case b := <-p.idle:
		return b
	default: // none idle, so create one
		return &Buffer{ID: int(p.created.Add(1))}
	}
}

func (p *Pool) Release(b *Buffer) {
	b.Text.Reset() // clear the state before the next user sees it
	select {
	case p.idle <- b:
	default: // the pool is full, so drop the item
	}
}

func main() {
	pool := NewPool(2)
	for i := 1; i <= 3; i++ {
		func() {
			b := pool.Acquire()
			defer pool.Release(b) // runs even if the body panics
			fmt.Fprintf(&b.Text, "request %d", i)
			fmt.Printf("request %d -> buffer #%d: %s\n", i, b.ID, b.Text.String())
		}()
	}

	held := []*Buffer{pool.Acquire(), pool.Acquire(), pool.Acquire()}
	ids := []string{}
	for _, b := range held {
		ids = append(ids, fmt.Sprintf("#%d", b.ID))
	}
	slices.Sort(ids)
	fmt.Println("3 at once -> buffers", strings.Join(ids, " "))
	for _, b := range held {
		pool.Release(b)
	}
	fmt.Println("idle after release:", len(pool.idle))
	fmt.Println("created:", pool.created.Load())
}
```

```output
request 1 -> buffer #1: request 1
request 2 -> buffer #1: request 2
request 3 -> buffer #1: request 3
3 at once -> buffers #1 #2 #3
idle after release: 2
created: 3
```

- Verdict: stdlib. `sync.Pool` is the standard pool, but it may drop any item at any time, so it suits temporary buffers and not a fixed set of costly objects. The example uses a buffered channel, which keeps its items.
- `defer` returns the item at the end of the function, as a `finally` block does.
- `database/sql` keeps a pool of connections inside each `*sql.DB`.
