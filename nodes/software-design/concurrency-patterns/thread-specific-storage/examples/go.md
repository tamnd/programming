```go run id=tss
package main

import (
	"fmt"
	"runtime"
	"strings"
	"sync"
	"sync/atomic"
)

// callState holds the per-goroutine data that errno holds per thread in C.
type callState struct {
	lastError int
}

func fakeCall(s *callState, code int) int {
	s.lastError = code // the call sets this goroutine's error
	runtime.Gosched()  // let another goroutine run between the set and the test
	return -1
}

func main() {
	var wrongReads atomic.Int64
	seen := make([]int, 4)
	var wg sync.WaitGroup
	for index := 0; index < 4; index++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			state := &callState{} // owned by this goroutine only
			code := index + 1
			for n := 0; n < 1000; n++ {
				if fakeCall(state, code) == -1 && state.lastError != code {
					wrongReads.Add(1)
				}
			}
			seen[index] = state.lastError
		}()
	}
	wg.Wait()
	fmt.Println("threads:", len(seen))
	fmt.Println("calls per thread:", 1000)
	fmt.Println("reads that saw another thread's error:", wrongReads.Load())
	codes := make([]string, len(seen))
	for i, c := range seen {
		codes[i] = fmt.Sprint(c)
	}
	fmt.Println("error code seen by each thread: [" + strings.Join(codes, ", ") + "]")
}
```

```output
threads: 4
calls per thread: 1000
reads that saw another thread's error: 0
error code seen by each thread: [1, 2, 3, 4]
```

- Verdict: idiom. Each goroutine creates its own `callState` and passes it as a parameter, the explicit form that the pattern paper gives as the alternative.
