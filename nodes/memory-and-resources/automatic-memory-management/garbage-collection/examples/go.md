```go run id=cycle
package main

import (
	"fmt"
	"runtime"
	"weak"
)

type item struct{ peer *item }

func main() {
	a, b := &item{}, &item{}
	a.peer, b.peer = b, a
	probe := weak.Make(a)
	fmt.Println("alive while referenced:", probe.Value() != nil)
	a, b = nil, nil
	runtime.GC()
	fmt.Println("collected after gc:", probe.Value() == nil)
}
```

```output
alive while referenced: true
collected after gc: true
```

- Verdict: native. The Go collector is a concurrent mark-sweep collector. It does not move objects.
- `runtime.GC()` runs a full collection and blocks the caller until it ends.
- `weak.Make` returns a `weak.Pointer`. Its `Value` method returns `nil` after the object has been collected.
- `GOGC` sets how much the heap can grow between collections, and `GOMEMLIMIT` sets a soft limit on the total memory of the runtime.
