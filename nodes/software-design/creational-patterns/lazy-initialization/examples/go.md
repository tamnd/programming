```go run id=lazy
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
)

type Settings struct {
	Config func() map[string]int
	loads  atomic.Int32
}

func NewSettings() *Settings {
	s := &Settings{}
	// OnceValue returns a function that calls load only once, even when
	// many goroutines call it at the same time.
	s.Config = sync.OnceValue(func() map[string]int {
		fmt.Println("loading config") // stands for a slow read of a file
		s.loads.Add(1)
		return map[string]int{"timeout": 30}
	})
	return s
}

func main() {
	s := NewSettings()
	fmt.Println("settings created, loads =", s.loads.Load())
	fmt.Println("timeout =", s.Config()["timeout"])
	fmt.Println("timeout =", s.Config()["timeout"])
	fmt.Println("loads =", s.loads.Load())

	shared := NewSettings()
	start := make(chan struct{})
	var wg sync.WaitGroup
	for range 8 {
		wg.Add(1)
		go func() {
			defer wg.Done()
			<-start // all eight goroutines ask at the same time
			shared.Config()
		}()
	}
	close(start)
	wg.Wait()
	fmt.Println("8 goroutines: loads =", shared.loads.Load())
}
```

```output
settings created, loads = 0
loading config
timeout = 30
timeout = 30
loads = 1
loading config
8 goroutines: loads = 1
```

- Verdict: stdlib. `sync.Once` runs an action exactly once, and `sync.OnceValue` wraps a function that returns a value. If the function panics, every call panics with the same value.
- A package-level variable is initialized before `main` runs, so it is not lazy. Use `sync.OnceValue` for a global that is costly and may not be needed.
