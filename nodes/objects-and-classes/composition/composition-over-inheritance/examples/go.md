```go run id=embedding-and-fields
package main

import (
	"fmt"
	"sort"
	"strings"
)

// CountingSet has-a map and counts the elements that callers try to add.
type CountingSet struct {
	items map[string]bool
	Added int
}

func NewCountingSet() *CountingSet { return &CountingSet{items: map[string]bool{}} }

func (s *CountingSet) Add(x string) {
	s.Added++
	s.items[x] = true
}

func (s *CountingSet) AddAll(xs ...string) {
	for _, x := range xs {
		s.Add(x)
	}
}

func (s *CountingSet) Len() int { return len(s.items) }

// Logger writes through a Writer, so any destination works.
type Writer interface{ Write(msg string) }

type Stdout struct{}

func (Stdout) Write(msg string) { fmt.Println(msg) }

type Upper struct{ Next Writer }

func (u Upper) Write(msg string) { u.Next.Write(strings.ToUpper(msg)) }

type Logger struct {
	Out    Writer
	Prefix string
}

func (l *Logger) Log(msg string) { l.Out.Write(l.Prefix + msg) }

// Service embeds *Logger: the methods of Logger are promoted to Service.
type Service struct {
	*Logger
	tags *CountingSet // a named field: its methods are not promoted
}

func main() {
	s := NewCountingSet()
	s.Add("a")
	s.AddAll("b", "c", "a")
	keys := make([]string, 0, s.Len())
	for k := range s.items {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	fmt.Println(s.Added, s.Len(), keys)

	svc := Service{Logger: &Logger{Out: Stdout{}, Prefix: "svc: "}, tags: s}
	svc.Log("ready") // a promoted method, the same as svc.Logger.Log
	svc.Out = Upper{Next: Stdout{}}
	svc.Log("ready")
	fmt.Println(svc.tags.Len())
}
```

```output for=embedding-and-fields
4 3 [a b c]
svc: ready
SVC: READY
3
```

```go wrong id=embedding-is-not-override
package main

import "fmt"

type Animal struct{}

func (Animal) Name() string { return "animal" }

func (a Animal) Greet() string { return "I am an " + a.Name() }

// Dog embeds Animal and defines its own Name.
type Dog struct{ Animal }

func (Dog) Name() string { return "dog" }

func main() {
	d := Dog{}
	fmt.Println(d.Name())
	fmt.Println(d.Greet()) // Greet runs on the embedded Animal, so it calls Animal.Name
}
```

```output for=embedding-is-not-override
dog
I am an animal
```

- Verdict: native. Go has no inheritance, and composition is the only way to build a type from other types. A named field such as `tags` holds a part and promotes nothing. An embedded field such as `*Logger` promotes the methods of the part to the outer type, so `svc.Log` works and is the same call as `svc.Logger.Log`. `Logger` writes through the `Writer` interface, so the destination can change at run time.
- The wrong block expects embedding to act like inheritance. `Dog` embeds `Animal` and defines its own `Name`, but `Greet` is a method of `Animal`, and its receiver is the embedded `Animal`, so it calls `Animal.Name`. Embedding forwards calls; it does not dispatch back to the outer type.
