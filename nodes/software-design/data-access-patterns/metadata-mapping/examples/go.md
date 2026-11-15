```go run id=generic-mapper-from-metadata
package main

import (
	"fmt"
	"reflect"
	"sort"
	"strings"
)

type row map[string]any

// A stand-in for the database: each table maps an id to a row of columns.
var db = map[string]map[int]row{"player": {}, "club": {}}

type Player struct {
	ID            int
	Name, Country string
}

func (p Player) String() string {
	return fmt.Sprintf("Player(id=%d, name=%s, country=%s)", p.ID, p.Name, p.Country)
}

type Club struct {
	ID         int
	Name, City string
}

func (c Club) String() string {
	return fmt.Sprintf("Club(id=%d, name=%s, city=%s)", c.ID, c.Name, c.City)
}

type columnMap struct{ Field, Column string }

// dataMap is the metadata: the type, the table and the pairs of field and column.
type dataMap struct {
	Type    reflect.Type
	Table   string
	Columns []columnMap
}

func (m dataMap) describe() string {
	var pairs []string
	for _, c := range m.Columns {
		// Go field names start with a capital letter; the output uses lower case.
		pairs = append(pairs, strings.ToLower(c.Field)+" -> "+c.Column)
	}
	return fmt.Sprintf("%s -> table %s: %s", m.Type.Name(), m.Table, strings.Join(pairs, ", "))
}

var maps = []dataMap{
	{reflect.TypeOf(Player{}), "player", []columnMap{{"ID", "id"}, {"Name", "name"}, {"Country", "nation"}}},
	{reflect.TypeOf(Club{}), "club", []columnMap{{"ID", "id"}, {"Name", "title"}, {"City", "city"}}},
}

func mapFor(t reflect.Type) dataMap {
	for _, m := range maps {
		if m.Type == t {
			return m
		}
	}
	panic("no data map for " + t.Name())
}

// insert, find and update are generic: they read the metadata and use reflection on the fields.
func insert(obj any) string {
	v := reflect.ValueOf(obj).Elem()
	m := mapFor(v.Type())
	var cols, marks []string
	r := row{}
	for _, c := range m.Columns {
		cols = append(cols, c.Column)
		marks = append(marks, "?")
		r[c.Column] = v.FieldByName(c.Field).Interface()
	}
	db[m.Table][v.FieldByName("ID").Interface().(int)] = r
	return fmt.Sprintf("insert into %s (%s) values (%s)", m.Table, strings.Join(cols, ", "), strings.Join(marks, ", "))
}

func find(obj any, id int) {
	v := reflect.ValueOf(obj).Elem()
	m := mapFor(v.Type())
	r := db[m.Table][id]
	for _, c := range m.Columns {
		v.FieldByName(c.Field).Set(reflect.ValueOf(r[c.Column]))
	}
}

func update(obj any) string {
	v := reflect.ValueOf(obj).Elem()
	m := mapFor(v.Type())
	r := db[m.Table][v.FieldByName("ID").Interface().(int)]
	var sets []string
	for _, c := range m.Columns {
		if c.Field == "ID" {
			continue
		}
		r[c.Column] = v.FieldByName(c.Field).Interface()
		sets = append(sets, c.Column+" = ?")
	}
	return fmt.Sprintf("update %s set %s where id = ?", m.Table, strings.Join(sets, ", "))
}

func main() {
	for _, m := range maps {
		fmt.Println("map", m.describe())
	}
	fmt.Println("insert:", insert(&Player{1, "Ann", "NZ"}))
	fmt.Println("insert:", insert(&Club{1, "Lions", "Leeds"}))
	var p Player
	find(&p, 1)
	fmt.Println("find player 1:", p)
	var c Club
	find(&c, 1)
	fmt.Println("find club 1:", c)
	p.Country = "AU"
	fmt.Println("update:", update(&p))
	for _, m := range maps {
		ids := make([]int, 0, len(db[m.Table]))
		for id := range db[m.Table] {
			ids = append(ids, id)
		}
		sort.Ints(ids)
		for _, id := range ids {
			var vs []string
			for _, c := range m.Columns {
				vs = append(vs, fmt.Sprint(db[m.Table][id][c.Column]))
			}
			fmt.Printf("row in %s: %s\n", m.Table, strings.Join(vs, " "))
		}
	}
}
```

```output
map Player -> table player: id -> id, name -> name, country -> nation
map Club -> table club: id -> id, name -> title, city -> city
insert: insert into player (id, name, nation) values (?, ?, ?)
insert: insert into club (id, title, city) values (?, ?, ?)
find player 1: Player(id=1, name=Ann, country=NZ)
find club 1: Club(id=1, name=Lions, city=Leeds)
update: update player set name = ?, nation = ? where id = ?
row in player: 1 Ann AU
row in club: 1 Lions Leeds
```

- Verdict: idiom. The `reflect` package gives `FieldByName`, `Interface` and `Set`, and `find` takes a pointer so that it can set the fields. Go field names start with a capital letter, so `describe` prints them in lower case.
- Many Go libraries keep this metadata in struct tags, such as `db:"nation"`, and read the tags with `reflect`.
