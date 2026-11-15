```go run id=table-per-class
package main

import (
	"fmt"
	"sort"
	"strings"
)

// One table for each class, all keyed by the same id.
var (
	playerTable     = map[int64]string{}
	footballerTable = map[int64]string{}
	cricketerTable  = map[int64]int{}
	bowlerTable     = map[int64]int{}
	lastID          int64
)

// Player is the interface of the hierarchy; Go has no class inheritance, so
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

// insert writes one row in the table of each class on the path from the root.
func insert(p Player) int64 {
	lastID++
	switch v := p.(type) {
	case *Footballer:
		v.ID = lastID
		playerTable[lastID] = v.Name
		footballerTable[lastID] = v.Club
	case *Cricketer:
		v.ID = lastID
		playerTable[lastID] = v.Name
		cricketerTable[lastID] = v.Runs
	case *Bowler:
		v.ID = lastID
		playerTable[lastID] = v.Name
		cricketerTable[lastID] = v.Runs
		bowlerTable[lastID] = v.Wickets
	}
	return lastID
}

// find looks for the key in the tables of the subclasses, because the root row has no type.
func find(id int64) (Player, []string) {
	name := playerTable[id]
	if club, ok := footballerTable[id]; ok {
		return &Footballer{id, name, club}, []string{"player", "footballer"}
	}
	runs := cricketerTable[id]
	if wickets, ok := bowlerTable[id]; ok {
		return &Bowler{Cricketer{id, name, runs}, wickets}, []string{"player", "cricketer", "bowler"}
	}
	return &Cricketer{id, name, runs}, []string{"player", "cricketer"}
}

func deletePlayer(id int64) int {
	count := 0
	if _, ok := bowlerTable[id]; ok { // Children first.
		delete(bowlerTable, id)
		count++
	}
	if _, ok := cricketerTable[id]; ok {
		delete(cricketerTable, id)
		count++
	}
	if _, ok := footballerTable[id]; ok {
		delete(footballerTable, id)
		count++
	}
	if _, ok := playerTable[id]; ok {
		delete(playerTable, id)
		count++
	}
	return count
}

func show[V any](table map[int64]V) string {
	keys := make([]int64, 0, len(table))
	for k := range table {
		keys = append(keys, k)
	}
	sort.Slice(keys, func(i, j int) bool { return keys[i] < keys[j] })
	parts := make([]string, len(keys))
	for i, k := range keys {
		parts[i] = fmt.Sprint(k, " ", table[k])
	}
	return strings.Join(parts, " | ")
}

func main() {
	insert(&Footballer{Name: "Ann", Club: "Lions"})
	insert(&Cricketer{Name: "Bo", Runs: 412})
	cy := insert(&Bowler{Cricketer{Name: "Cy", Runs: 95}, 31})
	fmt.Println("player:", show(playerTable))
	fmt.Println("footballer:", show(footballerTable))
	fmt.Println("cricketer:", show(cricketerTable))
	fmt.Println("bowler:", show(bowlerTable))
	for id := int64(1); id <= 3; id++ {
		p, used := find(id)
		fmt.Printf("loaded: %s (tables: %s)\n", p.Describe(), strings.Join(used, ", "))
	}
	var names []string
	for id := int64(1); id <= lastID; id++ {
		if _, ok := cricketerTable[id]; ok {
			names = append(names, playerTable[id])
		}
	}
	fmt.Println("cricketers:", strings.Join(names, ", "))
	fmt.Println("delete Cy: rows deleted", deletePlayer(cy))
}
```

```output
player: 1 Ann | 2 Bo | 3 Cy
footballer: 1 Lions
cricketer: 2 412 | 3 95
bowler: 3 31
loaded: Footballer Ann, club Lions (tables: player, footballer)
loaded: Cricketer Bo, runs 412 (tables: player, cricketer)
loaded: Bowler Cy, runs 95, wickets 31 (tables: player, cricketer, bowler)
cricketers: Bo, Cy
delete Cy: rows deleted 3
```

- Verdict: idiom. Go has no class inheritance, so `Bowler` embeds `Cricketer`, and a type switch in `insert` writes the rows for each type. `find` reads the maps with the two-value form to test for a row.
- `show` is generic over the value type of the map and sorts the keys, because Go maps have no order.
