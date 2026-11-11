```go run id=zero-values-and-new-functions
package main

import (
	"bytes"
	"errors"
	"fmt"
	"sync"
	"time"
)

// The zero value is ready to use: no constructor needed.
type Counter struct {
	mu sync.Mutex // the zero Mutex is unlocked
	n  map[string]int
}

func (c *Counter) Add(key string) {
	c.mu.Lock()
	defer c.mu.Unlock()
	if c.n == nil { // create the map lazily, so the zero Counter works
		c.n = map[string]int{}
	}
	c.n[key]++
}

// A constructor is an ordinary function, by convention named NewX.
type Range struct{ lo, hi int }

func NewRange(lo, hi int) (*Range, error) {
	if lo > hi {
		return nil, fmt.Errorf("empty range %d..%d", lo, hi)
	}
	return &Range{lo, hi}, nil
}

// Functional options give a constructor optional settings with defaults.
type Server struct {
	addr    string
	timeout time.Duration
}

type Option func(*Server)

func WithTimeout(d time.Duration) Option { return func(s *Server) { s.timeout = d } }

func NewServer(addr string, opts ...Option) (*Server, error) {
	if addr == "" {
		return nil, errors.New("empty address")
	}
	s := &Server{addr: addr, timeout: 30 * time.Second}
	for _, opt := range opts {
		opt(s)
	}
	return s, nil
}

func main() {
	var b bytes.Buffer // zero value: an empty buffer
	b.WriteString("ready")
	var c Counter
	c.Add("x")
	c.Add("x")
	fmt.Println(b.String(), c.n["x"])

	r, err := NewRange(3, 7)
	fmt.Println(*r, err)
	_, err = NewRange(9, 1)
	fmt.Println(err)

	s, _ := NewServer(":8080", WithTimeout(5*time.Second))
	fmt.Println(s.addr, s.timeout)
}
```

```output for=zero-values-and-new-functions
ready 2
{3 7} <nil>
empty range 9..1
:8080 5s
```

```go fails expect=panic id=nil-map
package main

import "fmt"

type Index struct {
	words map[string]int // nil in the zero value
}

func (ix *Index) Add(w string) { ix.words[w]++ }

func main() {
	var ix Index // no constructor ran
	ix.Add("go")
	fmt.Println(ix.words)
}
```

- Verdict: idiom. Go has no constructors. Every variable starts at the zero value of its type, and good Go types make that value useful: an empty `bytes.Buffer` is ready to write, and a zero `sync.Mutex` is unlocked. When a type needs set-up or checks, its package gives a function by convention named `NewX`, which returns a pointer and often an error. Functional options pass optional settings to such a function, with defaults set first.
- The failing block uses the zero value of a struct that holds a map. A zero map is `nil`: it can be read but not written, so the first `Add` panics. Create the map in a `NewIndex` function, or create it lazily in the method, as `Counter` does in the run block.
