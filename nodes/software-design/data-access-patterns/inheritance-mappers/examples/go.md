```go run id=mapper-for-each-class
package main

import (
	"fmt"
	"sort"
	"strings"
)

type row map[string]any

// A stand-in for the database: one player table with a type column.
var (
	playerTable       = map[int64]row{}
	nextID      int64 = 1
)

// PlayerBase holds the fields of Player. Go has no class inheritance, so the
// domain types embed it, and Bowler embeds Cricketer.
type PlayerBase struct {
	ID   int64
	Name string
}

func (p *PlayerBase) base() *PlayerBase { return p }

type Player interface {
	base() *PlayerBase
	Describe() string
}

type Footballer struct {
	PlayerBase
	Club string
}

type Cricketer struct {
	PlayerBase
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

// concreteMapper is what the generic insert and find need from each mapper.
type concreteMapper interface {
	typeCode() string
	name() string
	chain() []string
	create() Player
	save(p Player, r row, log *[]string)
	load(p Player, r row)
}

// AbstractPlayerMapper maps the fields of Player. Each mapper embeds the mapper
// of its superclass and calls it for the inherited fields.
type AbstractPlayerMapper struct{}

func (AbstractPlayerMapper) chain() []string { return []string{"AbstractPlayerMapper"} }
func (AbstractPlayerMapper) save(p Player, r row, log *[]string) {
	r["name"] = p.base().Name
	*log = append(*log, "name (Player)")
}
func (AbstractPlayerMapper) load(p Player, r row) { p.base().Name = r["name"].(string) }

type FootballerMapper struct{ AbstractPlayerMapper }

func (FootballerMapper) typeCode() string { return "footballer" }
func (FootballerMapper) name() string     { return "FootballerMapper" }
func (m FootballerMapper) chain() []string {
	return append([]string{m.name()}, m.AbstractPlayerMapper.chain()...)
}
func (FootballerMapper) create() Player { return &Footballer{} }
func (m FootballerMapper) save(p Player, r row, log *[]string) {
	m.AbstractPlayerMapper.save(p, r, log)
	r["club"] = p.(*Footballer).Club
	*log = append(*log, "club (Footballer)")
}
func (m FootballerMapper) load(p Player, r row) {
	m.AbstractPlayerMapper.load(p, r)
	p.(*Footballer).Club = r["club"].(string)
}

type CricketerMapper struct{ AbstractPlayerMapper }

func (CricketerMapper) typeCode() string { return "cricketer" }
func (CricketerMapper) name() string     { return "CricketerMapper" }
func (m CricketerMapper) chain() []string {
	return append([]string{m.name()}, m.AbstractPlayerMapper.chain()...)
}
func (CricketerMapper) create() Player { return &Cricketer{} }

// cricketer gives the Cricketer part of a Cricketer or a Bowler.
func cricketer(p Player) *Cricketer {
	if b, ok := p.(*Bowler); ok {
		return &b.Cricketer
	}
	return p.(*Cricketer)
}
func (m CricketerMapper) save(p Player, r row, log *[]string) {
	m.AbstractPlayerMapper.save(p, r, log)
	r["runs"] = cricketer(p).Runs
	*log = append(*log, "runs (Cricketer)")
}
func (m CricketerMapper) load(p Player, r row) {
	m.AbstractPlayerMapper.load(p, r)
	cricketer(p).Runs = r["runs"].(int)
}

type BowlerMapper struct{ CricketerMapper }

func (BowlerMapper) typeCode() string { return "bowler" }
func (BowlerMapper) name() string     { return "BowlerMapper" }
func (m BowlerMapper) chain() []string {
	return append([]string{m.name()}, m.CricketerMapper.chain()...)
}
func (BowlerMapper) create() Player { return &Bowler{} }
func (m BowlerMapper) save(p Player, r row, log *[]string) {
	m.CricketerMapper.save(p, r, log)
	r["wickets"] = p.(*Bowler).Wickets
	*log = append(*log, "wickets (Bowler)")
}
func (m BowlerMapper) load(p Player, r row) {
	m.CricketerMapper.load(p, r)
	p.(*Bowler).Wickets = r["wickets"].(int)
}

func insert(m concreteMapper, p Player) []string {
	r := row{"type": m.typeCode()}
	var log []string
	m.save(p, r, &log)
	p.base().ID = nextID
	playerTable[nextID] = r
	nextID++
	return log
}

func find(m concreteMapper, id int64, r row) Player {
	p := m.create()
	p.base().ID = id
	m.load(p, r)
	return p
}

// PlayerMapper is the mapper for the whole hierarchy: it chooses the concrete
// mapper and delegates to it.
type PlayerMapper struct {
	order  []string
	byType map[string]concreteMapper
}

func newPlayerMapper() *PlayerMapper {
	pm := &PlayerMapper{byType: map[string]concreteMapper{}}
	for _, m := range []concreteMapper{FootballerMapper{}, CricketerMapper{}, BowlerMapper{}} {
		pm.order = append(pm.order, m.typeCode())
		pm.byType[m.typeCode()] = m
	}
	return pm
}

func (pm *PlayerMapper) forObject(p Player) concreteMapper {
	switch p.(type) {
	case *Footballer:
		return pm.byType["footballer"]
	case *Bowler:
		return pm.byType["bowler"]
	default:
		return pm.byType["cricketer"]
	}
}

func (pm *PlayerMapper) insert(p Player) (concreteMapper, []string) {
	m := pm.forObject(p)
	return m, insert(m, p)
}

func (pm *PlayerMapper) find(id int64) (concreteMapper, Player) {
	r := playerTable[id]
	m := pm.byType[r["type"].(string)]
	return m, find(m, id, r)
}

func (pm *PlayerMapper) findAll() []Player {
	ids := make([]int64, 0, len(playerTable))
	for id := range playerTable {
		ids = append(ids, id)
	}
	sort.Slice(ids, func(i, j int) bool { return ids[i] < ids[j] })
	var out []Player
	for _, id := range ids {
		r := playerTable[id]
		out = append(out, find(pm.byType[r["type"].(string)], id, r))
	}
	return out
}

func main() {
	players := newPlayerMapper()
	for _, t := range players.order {
		fmt.Printf("mapper chain for %s: %s\n", t, strings.Join(players.byType[t].chain(), ", "))
	}
	ann := &Footballer{PlayerBase{Name: "Ann"}, "Lions"}
	bo := &Cricketer{PlayerBase{Name: "Bo"}, 412}
	cy := &Bowler{Cricketer{PlayerBase{Name: "Cy"}, 95}, 31}
	for _, p := range []Player{ann, bo, cy} {
		m, log := players.insert(p)
		fmt.Printf("insert %s: %s saved %s\n", p.base().Name, m.name(), strings.Join(log, ", "))
	}
	m, p := players.find(3)
	fmt.Printf("find 3: %s (mapper %s)\n", p.Describe(), m.name())
	var all []string
	for _, q := range players.findAll() {
		all = append(all, q.Describe())
	}
	fmt.Println("find all:", strings.Join(all, " | "))
}
```

```output
mapper chain for footballer: FootballerMapper, AbstractPlayerMapper
mapper chain for cricketer: CricketerMapper, AbstractPlayerMapper
mapper chain for bowler: BowlerMapper, CricketerMapper, AbstractPlayerMapper
insert Ann: FootballerMapper saved name (Player), club (Footballer)
insert Bo: CricketerMapper saved name (Player), runs (Cricketer)
insert Cy: BowlerMapper saved name (Player), runs (Cricketer), wickets (Bowler)
find 3: Bowler Cy, runs 95, wickets 31 (mapper BowlerMapper)
find all: Footballer Ann, club Lions | Cricketer Bo, runs 412 | Bowler Cy, runs 95, wickets 31
```

- Verdict: idiom. Go has no class inheritance, so each mapper struct embeds the mapper of its superclass and calls it by name, as in `m.CricketerMapper.save`. The free functions `insert` and `find` hold the shared steps and take the `concreteMapper` interface.
- `cricketer` gives the `Cricketer` part of a `*Cricketer` or a `*Bowler`, so `CricketerMapper` works for both types.
