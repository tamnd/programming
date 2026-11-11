```go run id=add-cleanup
package main

import (
	"fmt"
	"runtime"
)

type handle struct{ name string } // the underlying resource

func release(h *handle) { fmt.Println("release", h.name) }

type Conn struct {
	h       *handle
	cleanup runtime.Cleanup
}

func Open(name string) *Conn {
	c := &Conn{h: &handle{name}}
	// A safety net: the argument is the handle, not c, so c can still be collected.
	c.cleanup = runtime.AddCleanup(c, release, c.h)
	return c
}

func (c *Conn) Close() {
	if c.h == nil {
		return
	}
	c.cleanup.Stop() // the program closed it: cancel the safety net
	release(c.h)
	c.h = nil
}

func main() {
	a := Open("a")
	defer a.Close() // runs when main returns
	fmt.Println("using", a.h.name)

	b := Open("b")
	b.Close()
	b.Close() // a second call does nothing
	fmt.Println("end of main")
}
```

```output for=add-cleanup
using a
release b
end of main
release a
```

- Verdict: stdlib. Go has no destructors. `defer` runs `Close` when the function returns, and `runtime.AddCleanup` attaches a function that runs some time after an object is unreachable, in a separate goroutine. The cleanup receives the handle, not the `Conn`, because a cleanup that can reach its object keeps it alive forever. `Stop` cancels the cleanup after an explicit close. The older `runtime.SetFinalizer` passes the object itself to the finalizer, which can then bring it back to life.
