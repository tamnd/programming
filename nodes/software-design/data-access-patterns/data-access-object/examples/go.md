```go run id=two-daos-one-interface
package main

import (
	"fmt"
	"sort"
	"strconv"
	"strings"
)

// Player is the transfer object: plain data that goes in and out of the DAO.
type Player struct {
	ID      int
	Name    string
	Country string
}

// PlayerDao is the interface that the business code knows. It names no storage.
type PlayerDao interface {
	Insert(p Player)
	Find(id int) Player
	FindByCountry(country string) []Player
	FindAll() []Player
	Update(p Player)
	Delete(id int)
}

// MemoryPlayerDao keeps the data in a map from key to record.
type MemoryPlayerDao struct {
	rows map[int][2]string
}

func (d *MemoryPlayerDao) Insert(p Player) { d.rows[p.ID] = [2]string{p.Name, p.Country} }
func (d *MemoryPlayerDao) Update(p Player) { d.rows[p.ID] = [2]string{p.Name, p.Country} }
func (d *MemoryPlayerDao) Delete(id int)   { delete(d.rows, id) }

func (d *MemoryPlayerDao) Find(id int) Player {
	r := d.rows[id]
	return Player{id, r[0], r[1]}
}

func (d *MemoryPlayerDao) FindAll() []Player {
	var ids []int
	for id := range d.rows {
		ids = append(ids, id)
	}
	sort.Ints(ids)
	var out []Player
	for _, id := range ids {
		out = append(out, d.Find(id))
	}
	return out
}

func (d *MemoryPlayerDao) FindByCountry(country string) []Player {
	return byCountry(d.FindAll(), country)
}

// TextPlayerDao keeps the data as a flat file: one line "id;name;country" for each player.
type TextPlayerDao struct {
	lines []string
}

func parse(line string) Player {
	f := strings.Split(line, ";")
	id, _ := strconv.Atoi(f[0])
	return Player{id, f[1], f[2]}
}

func format(p Player) string { return fmt.Sprintf("%d;%s;%s", p.ID, p.Name, p.Country) }

func (d *TextPlayerDao) index(id int) int {
	for i, line := range d.lines {
		if parse(line).ID == id {
			return i
		}
	}
	panic("no player " + strconv.Itoa(id))
}

func (d *TextPlayerDao) Insert(p Player)    { d.lines = append(d.lines, format(p)) }
func (d *TextPlayerDao) Find(id int) Player { return parse(d.lines[d.index(id)]) }
func (d *TextPlayerDao) Update(p Player)    { d.lines[d.index(p.ID)] = format(p) }

func (d *TextPlayerDao) Delete(id int) {
	i := d.index(id)
	d.lines = append(d.lines[:i], d.lines[i+1:]...)
}

func (d *TextPlayerDao) FindAll() []Player {
	var out []Player
	for _, line := range d.lines {
		out = append(out, parse(line))
	}
	sort.Slice(out, func(i, j int) bool { return out[i].ID < out[j].ID })
	return out
}

func (d *TextPlayerDao) FindByCountry(country string) []Player {
	return byCountry(d.FindAll(), country)
}

func byCountry(all []Player, country string) []Player {
	var out []Player
	for _, p := range all {
		if p.Country == country {
			out = append(out, p)
		}
	}
	return out
}

// daoFactory is the one place that knows which storage the program uses.
func daoFactory(kind string) PlayerDao {
	if kind == "memory" {
		return &MemoryPlayerDao{rows: map[int][2]string{}}
	}
	return &TextPlayerDao{}
}

// PlayerService is the business object. It uses only the DAO interface.
type PlayerService struct {
	dao PlayerDao
}

func (s PlayerService) Transfer(id int, country string) {
	p := s.dao.Find(id)
	p.Country = country
	s.dao.Update(p)
}

func names(players []Player) string {
	var s []string
	for _, p := range players {
		s = append(s, p.Name+" "+p.Country)
	}
	return strings.Join(s, " | ")
}

func main() {
	daos := map[string]PlayerDao{}
	for _, kind := range []string{"memory", "text"} {
		dao := daoFactory(kind)
		daos[kind] = dao
		service := PlayerService{dao}
		fmt.Println("dao:", strings.TrimPrefix(fmt.Sprintf("%T", dao), "*main."))
		for _, p := range []Player{{1, "Ann", "NZ"}, {2, "Bo", "NZ"}, {3, "Cy", "GB"}} {
			dao.Insert(p)
		}
		fmt.Println("all:", names(dao.FindAll()))
		fmt.Println("find 1:", names([]Player{dao.Find(1)}))
		service.Transfer(1, "AU")
		fmt.Println("from NZ:", names(dao.FindByCountry("NZ")))
		fmt.Println("from AU:", names(dao.FindByCountry("AU")))
		dao.Delete(3)
		fmt.Println("all:", names(dao.FindAll()))
	}

	// The business code never saw this format; only TextPlayerDao knows it.
	text := daos["text"].(*TextPlayerDao)
	fmt.Println("lines in the text store:", strings.Join(text.lines, " | "))
}
```

```output
dao: MemoryPlayerDao
all: Ann NZ | Bo NZ | Cy GB
find 1: Ann NZ
from NZ: Bo NZ
from AU: Ann AU
all: Ann AU | Bo NZ
dao: TextPlayerDao
all: Ann NZ | Bo NZ | Cy GB
find 1: Ann NZ
from NZ: Bo NZ
from AU: Ann AU
all: Ann AU | Bo NZ
lines in the text store: 1;Ann;AU | 2;Bo;NZ
```

- Verdict: idiom. `PlayerDao` is a Go interface, and the two DAO types satisfy it with no declaration. The transfer object is a struct passed by value.
- A DAO for a database would take a `*sql.DB` and return an `error` with each result; the example leaves out the errors because the data is in memory.
