```go run id=guard
package main

import (
	"fmt"
	"sync"
)

var (
	mu    sync.Mutex
	table = map[string]int{}
)

func increment(path string) int {
	mu.Lock()
	defer mu.Unlock() // runs on every return
	if path == "" {
		return -1
	}
	table[path]++
	return 0
}

func main() {
	var wg sync.WaitGroup
	for i := 0; i < 4; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for j := 0; j < 1000; j++ {
				increment("/index")
			}
		}()
	}
	wg.Wait()
	fmt.Println("hits /index:", table["/index"])
	fmt.Println("bad path returns", increment(""))
	free := mu.TryLock()
	if free {
		mu.Unlock()
	}
	fmt.Println("lock free after the early return: " + map[bool]string{true: "True", false: "False"}[free])
	increment("/index")
	fmt.Println("hits /index:", table["/index"])
}
```

```output
hits /index: 4000
bad path returns -1
lock free after the early return: True
hits /index: 4001
```

- Verdict: idiom. Go has no destructors. `defer mu.Unlock()` right after `mu.Lock()` gives the same guarantee for the function scope.
