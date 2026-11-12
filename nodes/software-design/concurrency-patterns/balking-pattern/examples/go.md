```go run id=balking-pattern
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
)

type Document struct {
	mu      sync.Mutex
	write   func() // the slow part of a save
	changed bool
	saving  bool
	saves   int
}

func (d *Document) Edit() {
	d.mu.Lock()
	defer d.mu.Unlock()
	d.changed = true
}

// Save saves the document. It returns false at once if a save cannot start now.
func (d *Document) Save() bool {
	d.mu.Lock()
	if d.saving || !d.changed {
		d.mu.Unlock()
		return false // balk: the object is not in the right state
	}
	d.saving = true
	d.changed = false
	d.mu.Unlock()
	d.write()
	d.mu.Lock()
	defer d.mu.Unlock()
	d.saving = false
	d.saves++
	return true
}

func (d *Document) Saves() int {
	d.mu.Lock()
	defer d.mu.Unlock()
	return d.saves
}

func word(ran bool) string {
	if ran {
		return "ran"
	}
	return "balked"
}

func main() {
	started := make(chan struct{})
	release := make(chan struct{})
	var once sync.Once
	doc := &Document{write: func() {
		once.Do(func() { close(started) })
		<-release // the first save stays in progress until the other calls end
	}}
	doc.Edit()
	done := make(chan struct{})
	go func() {
		doc.Save()
		close(done)
	}()
	<-started

	var calls, balked atomic.Int64
	var wg sync.WaitGroup
	for i := 0; i < 7; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			calls.Add(1)
			if !doc.Save() {
				balked.Add(1)
			}
		}()
	}
	wg.Wait()
	close(release)
	<-done

	fmt.Println("calls during a save:", calls.Load())
	fmt.Println("calls that balked:", balked.Load())
	fmt.Println("saves run:", doc.Saves())
	fmt.Println("save with no changes:", word(doc.Save()))
	doc.Edit()
	fmt.Println("save after an edit:", word(doc.Save()))
	fmt.Println("saves run:", doc.Saves())
}
```

```output
calls during a save: 7
calls that balked: 7
saves run: 1
save with no changes: balked
save after an edit: ran
saves run: 2
```

- Verdict: stdlib. A `sync.Mutex` makes the test and the change of the `saving` flag one step.
