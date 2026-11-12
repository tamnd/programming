```go run id=breeds
package main

import "fmt"

type Breed struct {
	Name   string
	Health int
	Attack string
}

func (b *Breed) NewMonster() *Monster {
	return &Monster{breed: b, health: b.Health}
}

type Monster struct {
	breed  *Breed
	health int // each monster has its own health
}

func (m *Monster) Describe() string {
	return fmt.Sprintf("%s: %d health, attacks with %s", m.breed.Name, m.health, m.breed.Attack)
}

func main() {
	// New kinds are data, for example from a JSON file.
	breeds := []*Breed{
		{"goblin", 20, "a dagger"},
		{"dragon", 200, "fire"},
		{"wyvern", 120, "its tail"},
	}
	for _, b := range breeds {
		fmt.Println(b.NewMonster().Describe())
	}
}
```

```output for=breeds
goblin: 20 health, attacks with a dagger
dragon: 200 health, attacks with fire
wyvern: 120 health, attacks with its tail
```

- Verdict: idiom. A struct with one value for each kind, and a pointer to it in each monster.
