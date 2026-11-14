```go run id=one-object-per-row
package main

import (
	"fmt"
	"strings"
)

type Person struct {
	ID         int
	Name, City string
}

// A stand-in for the people table. The Go standard library has database/sql,
// but no driver for a real database. Each call of Select stands for one SELECT statement.
type personRow struct {
	id         int
	name, city string
}

type Database struct {
	rows    []personRow
	Selects int
}

func (db *Database) Select(where func(personRow) bool) []personRow {
	db.Selects++
	var out []personRow
	for _, r := range db.rows {
		if where(r) {
			out = append(out, r)
		}
	}
	return out
}

type PersonMapper struct {
	db          *Database
	identityMap map[int]*Person // primary key -> the one object for that row
}

func NewPersonMapper(db *Database) *PersonMapper {
	return &PersonMapper{db, map[int]*Person{}}
}

func (m *PersonMapper) Find(id int) (*Person, bool) {
	if p, ok := m.identityMap[id]; ok {
		return p, true // no query
	}
	rows := m.db.Select(func(r personRow) bool { return r.id == id })
	if len(rows) == 0 {
		return nil, false
	}
	return m.load(rows[0]), true
}

func (m *PersonMapper) FindByCity(city string) []*Person {
	// The map is keyed by id, so a query by city must run.
	var people []*Person
	for _, r := range m.db.Select(func(r personRow) bool { return r.city == city }) {
		people = append(people, m.load(r))
	}
	return people
}

func (m *PersonMapper) load(r personRow) *Person {
	p, ok := m.identityMap[r.id]
	if !ok {
		p = &Person{r.id, r.name, r.city}
		m.identityMap[r.id] = p
	}
	return p
}

func describe(people []*Person, withCity bool) string {
	var parts []string
	for _, p := range people {
		if withCity {
			parts = append(parts, p.Name+" in "+p.City)
		} else {
			parts = append(parts, p.Name)
		}
	}
	return strings.Join(parts, ", ")
}

func main() {
	db := &Database{rows: []personRow{{1, "Ann", "Oslo"}, {2, "Bo", "Oslo"}, {3, "Cy", "Bergen"}}}
	mapper := NewPersonMapper(db) // one mapper, and one map, for each session
	a, _ := mapper.Find(1)
	b, _ := mapper.Find(1)
	fmt.Printf("a == b: %t | selects: %d\n", a == b, db.Selects)

	oslo := mapper.FindByCity("Oslo")
	fmt.Printf("Oslo: %s | oslo[0] == a: %t | selects: %d\n", describe(oslo, false), oslo[0] == a, db.Selects)

	a.City = "Rome" // a change in memory, not saved yet
	fmt.Println("b.City:", b.City)
	fmt.Println("Oslo again:", describe(mapper.FindByCity("Oslo"), true))
	other, _ := NewPersonMapper(db).Find(1)
	fmt.Println("another session: same object:", other == a)
}
```

```output
a == b: true | selects: 1
Oslo: Ann, Bo | oslo[0] == a: true | selects: 2
b.City: Rome
Oslo again: Ann in Rome, Bo in Oslo
another session: same object: false
```

- Verdict: idiom. The map holds `*Person` values, so all callers share one struct for each row, and `==` on two pointers compares identity.
- A map of struct values in place of pointers would copy each struct on every read, and the change to `City` would not reach the other callers.
