```go run id=escape
package main

import "fmt"

type order struct {
	id    int
	total int
}

func newOrder(id int) *order {
	o := order{id: id, total: 100}
	return &o
}

func depth(n int) int {
	if n == 0 {
		return 0
	}
	return 1 + depth(n-1)
}

func main() {
	p := newOrder(7)
	fmt.Println(p.id, p.total)
	fmt.Println(depth(1_000_000))
}
```

```output
7 100
1000000
```

```go fails expect=exit:2 id=max-stack
package main

import (
	"fmt"
	"runtime/debug"
)

func descend(level int) int {
	return descend(level+1) + 1
}

func main() {
	debug.SetMaxStack(1 << 20)
	fmt.Println(descend(1))
}
```

- Verdict: native. The compiler decides where each variable lives. `newOrder` returns the address of a local, which is safe: `go build -gcflags=-m` reports `moved to heap: o`.
- A goroutine stack starts small and grows when it needs to, so a recursion depth of one million works here.
- The stack has a maximum, 1 GB on 64-bit systems by default. `debug.SetMaxStack` lowers it in the second block, and the runtime stops with `fatal error: stack overflow` and exit status 2.
