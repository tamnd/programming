```go run id=containers
package main

import (
	"fmt"
	"strings"
)

type Container struct {
	Name         string
	Refrigerated bool
	Clean        bool
}

// Spec is a function type with methods that combine specifications.
type Spec func(Container) bool

func (s Spec) And(o Spec) Spec { return func(c Container) bool { return s(c) && o(c) } }
func (s Spec) Or(o Spec) Spec  { return func(c Container) bool { return s(c) || o(c) } }
func (s Spec) Not() Spec       { return func(c Container) bool { return !s(c) } }

var containers = []Container{{"c1", true, true}, {"c2", true, false}, {"c3", false, true}}

func selectNames(s Spec) string {
	var names []string
	for _, c := range containers {
		if s(c) {
			names = append(names, c.Name)
		}
	}
	return strings.Join(names, ", ")
}

func main() {
	refrigerated := Spec(func(c Container) bool { return c.Refrigerated })
	clean := Spec(func(c Container) bool { return c.Clean })

	fmt.Println("meat:", selectNames(refrigerated.And(clean)))
	fmt.Println("not refrigerated:", selectNames(refrigerated.Not()))
	fmt.Println("refrigerated or clean:", selectNames(refrigerated.Or(clean)))
}
```

```output
meat: c1
not refrigerated: c3
refrigerated or clean: c1, c2, c3
```

- Verdict: idiom. `Spec` is a defined function type with the methods `And`, `Or` and `Not`, so a plain function literal becomes a specification by a conversion.
