```go run id=strategy
package main

import (
	"fmt"
	"sync"
)

// NullMutex is a lock for one goroutine: both methods do nothing.
type NullMutex struct{}

func (NullMutex) Lock()   {}
func (NullMutex) Unlock() {}

type FileCache struct {
	lock    sync.Locker // the locking strategy is a parameter
	files   map[string]string
	lookups int
}

func NewFileCache(lock sync.Locker) *FileCache {
	return &FileCache{lock: lock, files: map[string]string{}}
}

func (c *FileCache) Find(path string) string {
	c.lock.Lock()
	defer c.lock.Unlock()
	c.lookups++
	if _, ok := c.files[path]; !ok {
		c.files[path] = "mapped " + path
	}
	return c.files[path]
}

func main() {
	single := NewFileCache(NullMutex{})
	for _, path := range []string{"/a.html", "/b.html", "/a.html"} {
		single.Find(path)
	}
	fmt.Printf("Null_Mutex cache: %d files, %d lookups\n", len(single.files), single.lookups)

	multi := NewFileCache(&sync.Mutex{})
	paths := []string{"/a.html", "/b.html", "/c.html"}
	var wg sync.WaitGroup
	for w := 0; w < 4; w++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for i := 0; i < 1000; i++ {
				multi.Find(paths[i%3])
			}
		}()
	}
	wg.Wait()
	fmt.Printf("Thread_Mutex cache: %d files, %d lookups\n", len(multi.files), multi.lookups)
}
```

```output
Null_Mutex cache: 2 files, 3 lookups
Thread_Mutex cache: 3 files, 4000 lookups
```

- Verdict: stdlib. `sync.Locker` is the interface with `Lock` and `Unlock`, and `*sync.Mutex` implements it.
