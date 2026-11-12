```go run id=one-special-object
package main

import "fmt"

type Customer interface {
	Name() string
	DiscountPercent() int
	IsGuest() bool
}

type member struct {
	name     string
	discount int
}

func (m member) Name() string         { return m.name }
func (m member) DiscountPercent() int { return m.discount }
func (m member) IsGuest() bool        { return false }

// guest is the special case. It has no fields, so it cannot be changed.
type guest struct{}

func (guest) Name() string         { return "guest" }
func (guest) DiscountPercent() int { return 0 }
func (guest) IsGuest() bool        { return true }

var members = map[string]member{"c1": {"Ines Moreau", 10}}

func find(id string) Customer {
	if m, ok := members[id]; ok {
		return m
	}
	return guest{}
}

func labelBefore(id string) string {
	m, ok := members[id]
	if !ok {
		return "guest"
	}
	return m.name
}

func main() {
	for _, id := range []string{"c1", "c9"} {
		if find(id).Name() != labelBefore(id) {
			panic(id)
		}
	}
	if find("c9").DiscountPercent() != 0 || !find("c9").IsGuest() {
		panic("guest")
	}
	fmt.Println("introduce special case: tests pass")
}
```

```output
introduce special case: tests pass
```
