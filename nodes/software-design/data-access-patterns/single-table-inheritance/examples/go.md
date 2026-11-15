```go run id=one-table-for-hierarchy
package main

import (
	"fmt"
	"sort"
	"strings"
)

// row is one row of the single player table. A nil pointer is a null column.
type row struct {
	Type    string
	Name    string
	Club    *string
	Runs    *int
	Wickets *int
}

var (
	playerTable = map[int64]row{}
	lastID      int64
)

// Player is the interface of the hierarchy; Go has no class inheritance, so
// Bowler embeds Cricketer to reuse its fields.
type Player interface {
	Describe() string
	PlayerName() string
}

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
func (f *Footballer) PlayerName() string { return f.Name }
func (c *Cricketer) PlayerName() string  { return c.Name }

var subtypes = map[string][]string{"cricketer": {"cricketer", "bowler"}}

// insert writes the fields of each class; the columns of the other classes stay nil.
func insert(p Player) {
	lastID++
	switch v := p.(type) {
	case *Footballer:
		v.ID = lastID
		playerTable[lastID] = row{"footballer", v.Name, &v.Club, nil, nil}
	case *Bowler:
		v.ID = lastID
		playerTable[lastID] = row{"bowler", v.Name, nil, &v.Runs, &v.Wickets}
	case *Cricketer:
		v.ID = lastID
		playerTable[lastID] = row{"cricketer", v.Name, nil, &v.Runs, nil}
	}
}

func load(id int64, r row) Player {
	switch r.Type {
	case "footballer":
		return &Footballer{id, r.Name, *r.Club}
	case "cricketer":
		return &Cricketer{id, r.Name, *r.Runs}
	default:
		return &Bowler{Cricketer{id, r.Name, *r.Runs}, *r.Wickets}
	}
}

func ids() []int64 {
	keys := make([]int64, 0, len(playerTable))
	for k := range playerTable {
		keys = append(keys, k)
	}
	sort.Slice(keys, func(i, j int) bool { return keys[i] < keys[j] })
	return keys
}

func findAll(kind string) []Player {
	kinds := subtypes[kind]
	if kinds == nil && kind != "" {
		kinds = []string{kind}
	}
	var result []Player
	for _, id := range ids() {
		r := playerTable[id]
		match := kind == ""
		for _, k := range kinds {
			if r.Type == k {
				match = true
			}
		}
		if match {
			result = append(result, load(id, r))
		}
	}
	return result
}

func showString(p *string) string {
	if p == nil {
		return "null"
	}
	return *p
}

func showInt(p *int) string {
	if p == nil {
		return "null"
	}
	return fmt.Sprint(*p)
}

func main() {
	insert(&Footballer{Name: "Ann", Club: "Lions"})
	insert(&Cricketer{Name: "Bo", Runs: 412})
	insert(&Bowler{Cricketer{Name: "Cy", Runs: 95}, 31})
	for _, id := range ids() {
		r := playerTable[id]
		fmt.Println("row:", id, r.Type, r.Name, showString(r.Club), showInt(r.Runs), showInt(r.Wickets))
	}
	for _, p := range findAll("") {
		fmt.Println("loaded:", p.Describe())
	}
	var names []string
	for _, p := range findAll("cricketer") {
		names = append(names, p.PlayerName())
	}
	fmt.Println("cricketers:", strings.Join(names, ", "))
	nulls := 0
	for _, r := range playerTable {
		if r.Club == nil {
			nulls++
		}
		if r.Runs == nil {
			nulls++
		}
		if r.Wickets == nil {
			nulls++
		}
	}
	fmt.Printf("null cells in subclass columns: %d of %d\n", nulls, 3*len(playerTable))
}
```

```output
row: 1 footballer Ann Lions null null
row: 2 cricketer Bo null 412 null
row: 3 bowler Cy null 95 31
loaded: Footballer Ann, club Lions
loaded: Cricketer Bo, runs 412
loaded: Bowler Cy, runs 95, wickets 31
cricketers: Bo, Cy
null cells in subclass columns: 5 of 9
```

- Verdict: idiom. Go has no class inheritance, so the hierarchy is an interface, and `Bowler` embeds `Cricketer` to reuse its fields. A column that can be null is a pointer, and a type switch in `insert` chooses the type code.
- Go maps have no order, so `ids` sorts the keys before each scan of the table.
