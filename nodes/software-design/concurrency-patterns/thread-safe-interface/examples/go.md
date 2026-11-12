```go run id=interface
package main

import (
	"fmt"
	"sync"
)

type FileCache struct {
	mu       sync.Mutex // not reentrant
	files    map[string]string
	acquires int
}

// Interface methods: take the lock, then call an implementation method.
func (c *FileCache) Find(path string) string {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.acquires++
	return c.findI(path)
}

func (c *FileCache) Bind(path string) {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.acquires++
	c.bindI(path)
}

// Implementation methods: trust that the lock is held, never take it.
func (c *FileCache) findI(path string) string {
	file, ok := c.checkCacheI(path)
	if !ok {
		c.bindI(path) // not Bind: that would take the lock again
		file, _ = c.checkCacheI(path)
	}
	return file
}

func (c *FileCache) checkCacheI(path string) (string, bool) {
	file, ok := c.files[path]
	return file, ok
}

func (c *FileCache) bindI(path string) {
	c.files[path] = "mapped " + path
}

func main() {
	cache := &FileCache{files: map[string]string{}}
	cache.Bind("/index.html")
	paths := []string{"/a.html", "/b.html", "/c.html"}
	var wg sync.WaitGroup
	for w := 0; w < 4; w++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for i := 0; i < 1000; i++ {
				cache.Find(paths[i%3])
			}
		}()
	}
	wg.Wait()
	fmt.Println("find /a.html:", cache.Find("/a.html"))
	fmt.Println("files cached:", len(cache.files))
	fmt.Println("lock acquisitions:", cache.acquires)
}
```

```output
find /a.html: mapped /a.html
files cached: 4
lock acquisitions: 4002
```

- Verdict: idiom. Exported methods are the interface methods, and unexported methods are the implementation methods. `sync.Mutex` is not reentrant, so a call from `findI` to `Find` would block.
