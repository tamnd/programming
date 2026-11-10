```go run id=defer
package main

import (
	"errors"
	"fmt"
	"sync"
)

type resource struct{ name string }

func open(name string) *resource {
	fmt.Println("open", name)
	return &resource{name}
}

func (r *resource) Close() { fmt.Println("close", r.name) }

var (
	mu     sync.Mutex
	copies int
)

func copyFile(fail bool) (err error) {
	mu.Lock()
	defer mu.Unlock()
	source := open("source")
	defer source.Close()
	target := open("target")
	defer target.Close()
	if fail {
		return errors.New("disk full")
	}
	copies++
	return nil
}

func main() {
	fmt.Println("result:", copyFile(false))
	fmt.Println("result:", copyFile(true))
	fmt.Println("unlocked:", mu.TryLock(), "copies:", copies)
	mu.Unlock()
}
```

```output
open source
open target
close target
close source
result: <nil>
open source
open target
close target
close source
result: disk full
unlocked: true copies: 1
```

- Verdict: native. `defer` saves a call that runs when the surrounding function returns, also when it returns an error or panics. Deferred calls run in reverse order.
- `defer mu.Unlock()` right after `mu.Lock()` keeps the lock and its release together. Every return path of `copyFile` unlocks.
- The scope of `defer` is the function, not the block. Put the body of a loop in its own function when each iteration opens a resource.
- `os.Exit` ends the program without running deferred calls.
