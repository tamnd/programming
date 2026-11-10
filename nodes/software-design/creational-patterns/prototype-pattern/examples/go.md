```go run id=spawner
package main

import (
	"fmt"
	"slices"
	"strings"
)

type Prototype interface {
	Clone() Prototype
	Describe() string
}

type Monster struct {
	Name   string
	Health int
	Items  []string
}

func (m *Monster) Clone() Prototype {
	copy := *m                         // copies the fields; Items still shares its array
	copy.Items = slices.Clone(m.Items) // gives the copy its own array
	return &copy
}

func (m *Monster) Describe() string {
	return fmt.Sprintf("%s: health %d, items %s", m.Name, m.Health, strings.Join(m.Items, ", "))
}

type Trap struct {
	Name   string
	Damage int
}

func (t *Trap) Clone() Prototype {
	copy := *t // no field refers to shared data, so this copy is enough
	return &copy
}

func (t *Trap) Describe() string {
	return fmt.Sprintf("%s: damage %d", t.Name, t.Damage)
}

func main() {
	registry := map[string]Prototype{
		"goblin": &Monster{Name: "goblin", Health: 30, Items: []string{"dagger"}},
		"spikes": &Trap{Name: "spikes", Damage: 5},
	}
	spawn := func(key string) Prototype { return registry[key].Clone() }

	first := spawn("goblin").(*Monster)
	first.Items = append(first.Items, "torch")
	second := spawn("goblin")
	trap := spawn("spikes")
	for _, thing := range []Prototype{first, second, trap} {
		fmt.Println(thing.Describe())
	}
	fmt.Println("prototype", registry["goblin"].Describe())
}
```

```output for=spawner
goblin: health 30, items dagger, torch
goblin: health 30, items dagger
spikes: damage 5
prototype goblin: health 30, items dagger
```

- Verdict: idiom. An interface with a `Clone` method. The assignment `copy := *m` copies a struct by value, field by field.
- A slice is a small header with a pointer to an array, and a map value refers to a hash table. A struct copy copies the header and the reference, not the elements, so the method calls `slices.Clone`. `slices.Clone` and `maps.Clone` are shallow: they copy the elements by assignment.
- The node page shows the bug when `Clone` omits `slices.Clone` and a later `append` writes into the shared array.
- Go has no copy constructors and no `clone` in the language. Each type that holds a slice, a map or a pointer must write its own copy.
