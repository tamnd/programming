```go run id=thunks
package main

import (
	"fmt"
	"sync"
)

func load() int {
	fmt.Println("loading")
	return 21
}

func main() {
	plain := func() int { return load() }
	memo := sync.OnceValue(load)

	fmt.Println(plain() + plain())
	fmt.Println(memo() + memo())
}
```

```output for=thunks
loading
loading
42
loading
42
```

- Verdict: stdlib. A `func() T` value is a plain thunk. `sync.OnceValue` turns it into a memoized thunk that many goroutines can call at the same time, and `sync.OnceValues` does the same for a function that returns a value and an error.
