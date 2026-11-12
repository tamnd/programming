```go run id=immutable-object
package main

import (
	"fmt"
	"runtime"
	"strconv"
	"sync"
	"sync/atomic"
)

// Settings has unexported fields and no methods that change them.
type Settings struct {
	port int
	tag  string
}

func NewSettings(port int) *Settings {
	return &Settings{port: port, tag: "port " + strconv.Itoa(port)}
}

func (s *Settings) Port() int   { return s.port }
func (s *Settings) Tag() string { return s.tag }

func main() {
	first := NewSettings(8000)
	var current atomic.Pointer[Settings] // the writer replaces the pointer
	current.Store(first)
	var reads, mixed atomic.Int64
	var wg sync.WaitGroup
	for i := 0; i < 4; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for r := 0; r < 1000; r++ {
				s := current.Load() // one load gives a consistent snapshot
				runtime.Gosched()
				if s.Tag() != "port "+strconv.Itoa(s.Port()) {
					mixed.Add(1)
				}
				reads.Add(1)
			}
		}()
	}
	wg.Add(1)
	go func() {
		defer wg.Done()
		for port := 8001; port <= 8100; port++ {
			current.Store(NewSettings(port)) // a new object, never a change
			runtime.Gosched()
		}
	}()
	wg.Wait()
	unchanged := "False"
	if first.Port() == 8000 && first.Tag() == "port 8000" {
		unchanged = "True"
	}
	fmt.Println("reads:", reads.Load())
	fmt.Println("reads that saw a mixed pair:", mixed.Load())
	fmt.Println("settings published:", 1+current.Load().Port()-first.Port())
	fmt.Println("first settings unchanged:", unchanged)
}
```

```output
reads: 4000
reads that saw a mixed pair: 0
settings published: 101
first settings unchanged: True
```

- Verdict: idiom. `Settings` keeps its fields unexported and has only read methods, and `atomic.Pointer` publishes each new object.
