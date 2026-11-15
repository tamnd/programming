```go run id=person-registry
package main

import (
	"errors"
	"fmt"
	"sync"
)

type PersonFinder interface {
	Find(personID int) string
}

type tableFinder map[int]string

func (t tableFinder) Find(id int) string { return t[id] }

type stubFinder struct{}

func (stubFinder) Find(id int) string { return fmt.Sprintf("test person %d", id) }

// Registry is a well-known object with one explicit method for each service.
type Registry struct {
	PersonFinder PersonFinder
}

var (
	mu   sync.RWMutex
	sole Registry
)

// Install sets the registry. The start of the program, or a test, calls it.
func Install(r Registry) {
	mu.Lock()
	defer mu.Unlock()
	sole = r
}

// Finder returns the registered person finder.
func Finder() (PersonFinder, error) {
	mu.RLock()
	defer mu.RUnlock()
	if sole.PersonFinder == nil {
		return nil, errors.New("no person finder is registered")
	}
	return sole.PersonFinder, nil
}

// greet is code deep in the program, with no reference to a finder.
func greet(id int) (string, error) {
	f, err := Finder()
	if err != nil {
		return "", err
	}
	return "hello, " + f.Find(id), nil
}

func main() {
	Install(Registry{PersonFinder: tableFinder{1: "ann", 2: "bob"}})
	for _, id := range []int{1, 2} {
		s, _ := greet(id)
		fmt.Println(s)
	}

	Install(Registry{PersonFinder: stubFinder{}}) // a test installs a stub
	s, _ := greet(1)
	fmt.Println("in a test: " + s)

	Install(Registry{})
	if _, err := greet(1); err != nil {
		fmt.Println("with an empty registry:", err)
	}
}
```

```output
hello, ann
hello, bob
in a test: hello, test person 1
with an empty registry: no person finder is registered
```

- Verdict: idiom. A package-level variable holds the registry, and a `sync.RWMutex` protects it, because goroutines can read it while a test installs another one.
- `Finder` returns an error and not a nil interface, so the caller must handle an empty registry.
