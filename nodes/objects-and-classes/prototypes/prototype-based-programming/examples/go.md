```go run id=delegating-map
package main

import "fmt"

// Obj has its own slots and a parent to delegate to.
type Obj struct {
	parent *Obj
	slots  map[string]any
}

func New(parent *Obj) *Obj { return &Obj{parent, map[string]any{}} }

func (o *Obj) Get(name string) any {
	for x := o; x != nil; x = x.parent {
		if v, ok := x.slots[name]; ok {
			return v
		}
	}
	panic("no slot " + name)
}

func (o *Obj) Set(name string, v any) *Obj { o.slots[name] = v; return o }

func (o *Obj) Send(name string) string { return o.Get(name).(func(*Obj) string)(o) }

func main() {
	traits := New(nil).Set("describe", func(o *Obj) string {
		return fmt.Sprintf("(%v, %v)", o.Get("x"), o.Get("y"))
	})
	origin := New(traits).Set("x", 0).Set("y", 0)
	p := New(origin).Set("x", 3) // p delegates to origin

	fmt.Println(origin.Send("describe"), p.Send("describe"))
	traits.Set("describe", func(o *Obj) string {
		return fmt.Sprintf("<%v %v>", o.Get("x"), o.Get("y"))
	})
	fmt.Println(origin.Send("describe"), p.Send("describe"))
}
```

```output for=delegating-map
(0, 0) (3, 0)
<0 0> <3 0>
```

```go wrong id=shared-map
package main

import "fmt"

type Form struct {
	Name   string
	Fields map[string]bool
}

func main() {
	prototype := Form{Name: "form", Fields: map[string]bool{"id": true}}

	login := prototype // a struct copy: the map header is copied, not the map
	login.Name = "login"
	login.Fields["password"] = true

	fmt.Println(login.Name, len(login.Fields))
	fmt.Println(prototype.Name, len(prototype.Fields))
}
```

```output for=shared-map
login 2
form 2
```

- Verdict: absent. Go types are fixed at compile time, and struct embedding promotes the fields and methods of an embedded value, which is delegation decided by the compiler, not by a link that can change. The run block builds prototype objects from a map of slots and a parent pointer, with functions stored as values and a type assertion to call them.
- The wrong block clones a struct by assignment. The copy gets its own `Name`, but a map value is a reference to the map, so both structs share one map, and the new field shows in the prototype. Use `maps.Clone` for each map field, or write a clone method.
