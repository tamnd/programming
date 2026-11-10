```go run id=settings
package main

import (
	"fmt"
	"sync"
)

type Settings struct {
	mu    sync.Mutex
	level string
}

func (s *Settings) Level() string {
	s.mu.Lock()
	defer s.mu.Unlock()
	return s.level
}

func (s *Settings) SetLevel(level string) {
	s.mu.Lock()
	defer s.mu.Unlock()
	s.level = level
}

// GetSettings calls the function once, on the first call, even from many goroutines.
var GetSettings = sync.OnceValue(func() *Settings {
	fmt.Println("creating the settings")
	return &Settings{level: "info"}
})

func main() {
	first := GetSettings()
	fmt.Println("main:", first.Level())
	var seen *Settings
	var wg sync.WaitGroup
	wg.Go(func() {
		settings := GetSettings()
		fmt.Println("worker sets debug")
		settings.SetLevel("debug")
		seen = settings
	})
	wg.Wait()
	fmt.Println("main:", GetSettings().Level())
	fmt.Println("same instance:", seen == first)
}
```

```output
creating the settings
main: info
worker sets debug
main: debug
same instance: true
```

- Verdict: stdlib. `sync.OnceValue` wraps a function so that it runs once, and returns its result to every caller. Concurrent callers wait for the first call to finish.
- A package-level variable, `var settings = &Settings{level: "info"}`, is the eager form. Go initializes each package once, before `main` runs.
- The mutex protects the state of the instance. `OnceValue` only protects its creation.
- `sync.WaitGroup.Go` starts the goroutine and adds it to the group in one call.
