```go run id=table-per-concrete-class
package main

import (
	"fmt"
	"sort"
	"strings"
)

// One table for each concrete class, with all its fields, the inherited ones too.
type footballerRow struct {
	Name, Club string
}

type cricketerRow struct {
	Name string
	Runs int
}

type bowlerRow struct {
	Name          string
	Runs, Wickets int
}

var (
	footballerTable       = map[int64]footballerRow{}
	cricketerTable        = map[int64]cricketerRow{}
	bowlerTable           = map[int64]bowlerRow{}
	nextID          int64 = 1 // One key source for all the tables.
)

// Player is the interface of the hierarchy. Go has no class inheritance, so
// Bowler embeds Cricketer to reuse its fields.
type Player interface{ Describe() string }

type Footballer struct {
	ID   int64
	Name string
	Club string
}

type Cricketer struct {
	ID   int64
	Name string
	Runs int
}

type Bowler struct {
	Cricketer
	Wickets int
}

func (f *Footballer) Describe() string { return fmt.Sprintf("Footballer %s, club %s", f.Name, f.Club) }
func (c *Cricketer) Describe() string  { return fmt.Sprintf("Cricketer %s, runs %d", c.Name, c.Runs) }
func (b *Bowler) Describe() string {
	return fmt.Sprintf("Bowler %s, runs %d, wickets %d", b.Name, b.Runs, b.Wickets)
}

func insert(p Player) {
	id := nextID
	nextID++
	switch v := p.(type) {
	case *Footballer:
		v.ID = id
		footballerTable[id] = footballerRow{v.Name, v.Club}
	case *Cricketer:
		v.ID = id
		cricketerTable[id] = cricketerRow{v.Name, v.Runs}
	case *Bowler:
		v.ID = id
		bowlerTable[id] = bowlerRow{v.Name, v.Runs, v.Wickets}
	}
}

// find tries each table, because a key alone does not say which table holds the row.
func find(id int64) (Player, []string) {
	searched := []string{"footballer"}
	if r, ok := footballerTable[id]; ok {
		return &Footballer{id, r.Name, r.Club}, searched
	}
	searched = append(searched, "cricketer")
	if r, ok := cricketerTable[id]; ok {
		return &Cricketer{id, r.Name, r.Runs}, searched
	}
	searched = append(searched, "bowler")
	if r, ok := bowlerTable[id]; ok {
		return &Bowler{Cricketer{id, r.Name, r.Runs}, r.Wickets}, searched
	}
	return nil, searched
}

func sortedKeys[V any](m map[int64]V) []int64 {
	keys := make([]int64, 0, len(m))
	for k := range m {
		keys = append(keys, k)
	}
	sort.Slice(keys, func(i, j int) bool { return keys[i] < keys[j] })
	return keys
}

type summary struct {
	ID          int64
	Name, Table string
}

func main() {
	insert(&Footballer{Name: "Ann", Club: "Lions"})
	insert(&Cricketer{Name: "Bo", Runs: 412})
	insert(&Bowler{Cricketer{Name: "Cy", Runs: 95}, 31})
	var parts []string
	for _, id := range sortedKeys(footballerTable) {
		r := footballerTable[id]
		parts = append(parts, fmt.Sprintf("%d %s %s", id, r.Name, r.Club))
	}
	fmt.Println("footballer:", strings.Join(parts, " | "))
	parts = nil
	for _, id := range sortedKeys(cricketerTable) {
		r := cricketerTable[id]
		parts = append(parts, fmt.Sprintf("%d %s %d", id, r.Name, r.Runs))
	}
	fmt.Println("cricketer:", strings.Join(parts, " | "))
	parts = nil
	for _, id := range sortedKeys(bowlerTable) {
		r := bowlerTable[id]
		parts = append(parts, fmt.Sprintf("%d %s %d %d", id, r.Name, r.Runs, r.Wickets))
	}
	fmt.Println("bowler:", strings.Join(parts, " | "))
	p, searched := find(3)
	fmt.Printf("find 3: %s (tables searched: %s)\n", p.Describe(), strings.Join(searched, ", "))
	// The union of all the tables, as a polymorphic query does it.
	var everyone []summary
	for id, r := range footballerTable {
		everyone = append(everyone, summary{id, r.Name, "footballer"})
	}
	for id, r := range cricketerTable {
		everyone = append(everyone, summary{id, r.Name, "cricketer"})
	}
	for id, r := range bowlerTable {
		everyone = append(everyone, summary{id, r.Name, "bowler"})
	}
	sort.Slice(everyone, func(i, j int) bool { return everyone[i].ID < everyone[j].ID })
	parts = nil
	for _, s := range everyone {
		parts = append(parts, fmt.Sprintf("%d %s %s", s.ID, s.Name, s.Table))
	}
	fmt.Println("all players:", strings.Join(parts, " | "))
	var cricketers []string
	for _, r := range cricketerTable {
		cricketers = append(cricketers, r.Name)
	}
	for _, r := range bowlerTable {
		cricketers = append(cricketers, r.Name)
	}
	sort.Strings(cricketers)
	fmt.Println("cricketers:", strings.Join(cricketers, ", "))
	fmt.Println("next key:", nextID)
}
```

```output
footballer: 1 Ann Lions
cricketer: 2 Bo 412
bowler: 3 Cy 95 31
find 3: Bowler Cy, runs 95, wickets 31 (tables searched: footballer, cricketer, bowler)
all players: 1 Ann footballer | 2 Bo cricketer | 3 Cy bowler
cricketers: Bo, Cy
next key: 4
```

- Verdict: idiom. Go has no class inheritance, so `Bowler` embeds `Cricketer`, and a type switch in `insert` chooses the table. Each row struct holds all the fields of its type.
- `sortedKeys` is generic over the value type of the map, because Go maps have no order and the output must be stable.
