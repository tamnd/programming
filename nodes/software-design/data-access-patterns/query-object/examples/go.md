```go run id=query-object-to-sql
package main

import (
	"fmt"
	"sort"
	"strings"
)

type Player struct {
	ID      int
	Name    string
	Country string
	Goals   int
}

func (p Player) get(field string) any {
	switch field {
	case "id":
		return p.ID
	case "name":
		return p.Name
	case "country":
		return p.Country
	}
	return p.Goals
}

// DataMap is the metadata: the table and, for each field, its column.
type DataMap struct {
	Table   string
	Fields  []string
	Columns map[string]string
}

// Condition is a node of the query object: it can write itself as SQL or test an object.
type Condition interface {
	SQL(cols map[string]string, params *[]any) string
	Matches(p Player) bool
}

type Criterion struct {
	Op, Field string
	Value     any
}

func (c Criterion) SQL(cols map[string]string, params *[]any) string {
	col := cols[c.Field]
	if c.Op == "starts" {
		*params = append(*params, c.Value.(string)+"%")
		return col + " like ?"
	}
	*params = append(*params, c.Value)
	return col + " " + c.Op + " ?"
}

func (c Criterion) Matches(p Player) bool {
	v := p.get(c.Field)
	switch c.Op {
	case "=":
		return v == c.Value
	case ">":
		return v.(int) > c.Value.(int)
	}
	return strings.HasPrefix(v.(string), c.Value.(string))
}

// AnyOf is a group of conditions joined with "or".
type AnyOf []Condition

func (a AnyOf) SQL(cols map[string]string, params *[]any) string {
	var s []string
	for _, c := range a {
		s = append(s, c.SQL(cols, params))
	}
	return "(" + strings.Join(s, " or ") + ")"
}

func (a AnyOf) Matches(p Player) bool {
	for _, c := range a {
		if c.Matches(p) {
			return true
		}
	}
	return false
}

// Query is a query on fields. It knows nothing of tables and columns
// until ToSQL reads the data map.
type Query struct {
	criteria []Condition
	order    string
}

func (q *Query) Where(c Condition) *Query {
	q.criteria = append(q.criteria, c)
	return q
}

func (q *Query) OrderBy(f string) *Query {
	q.order = f
	return q
}

func (q *Query) ToSQL(m DataMap) (string, []any) {
	var cols []string
	for _, f := range m.Fields {
		cols = append(cols, m.Columns[f])
	}
	params := []any{}
	sql := "select " + strings.Join(cols, ", ") + " from " + m.Table
	if len(q.criteria) > 0 {
		var s []string
		for _, c := range q.criteria {
			s = append(s, c.SQL(m.Columns, &params))
		}
		sql += " where " + strings.Join(s, " and ")
	}
	if q.order != "" {
		sql += " order by " + m.Columns[q.order]
	}
	return sql, params
}

func (q *Query) Matches(p Player) bool {
	for _, c := range q.criteria {
		if !c.Matches(p) {
			return false
		}
	}
	return true
}

var players = []Player{{1, "Ann", "NZ", 14}, {2, "Bo", "NZ", 3}, {3, "Cy", "NZ", 21}, {4, "Dee", "AU", 9}, {5, "Eli", "GB", 12}}

func run(title string, q *Query, m DataMap) {
	sql, params := q.ToSQL(m)
	fmt.Println("query:", title)
	fmt.Println("sql:", sql)
	var shown []string
	for _, p := range params {
		if s, ok := p.(string); ok {
			shown = append(shown, "'"+s+"'")
		} else {
			shown = append(shown, fmt.Sprint(p))
		}
	}
	fmt.Printf("params: [%s]\n", strings.Join(shown, ", "))
	// No database here: the same query object is run on the objects in memory.
	var found []Player
	for _, p := range players {
		if q.Matches(p) {
			found = append(found, p)
		}
	}
	sort.Slice(found, func(i, j int) bool { return found[i].get(q.order).(string) < found[j].get(q.order).(string) })
	var rows, names []string
	for _, p := range found {
		rows = append(rows, fmt.Sprintf("%s (%s, %d)", p.Name, p.Country, p.Goals))
		names = append(names, p.Name)
	}
	fmt.Println("rows:", strings.Join(rows, " | "))
	fmt.Println("in memory:", strings.Join(names, " | "))
}

func main() {
	fields := []string{"id", "name", "country", "goals"}
	playerMap := DataMap{"player", fields, map[string]string{"id": "id", "name": "name", "country": "nation", "goals": "goals_scored"}}

	q1 := (&Query{}).Where(Criterion{"=", "country", "NZ"}).Where(Criterion{">", "goals", 10}).OrderBy("name")
	q2 := (&Query{}).Where(AnyOf{Criterion{"starts", "name", "B"}, Criterion{"=", "country", "AU"}}).OrderBy("name")
	run("from NZ and more than 10 goals", q1, playerMap)
	run("name starts with B or from AU", q2, playerMap)

	// The column nation is renamed. Only the data map changes; the query does not.
	newCols := map[string]string{}
	for k, v := range playerMap.Columns {
		newCols[k] = v
	}
	newCols["country"] = "country_code"
	run("from NZ and more than 10 goals, after the rename", q1, DataMap{"player", fields, newCols})
}
```

```output
query: from NZ and more than 10 goals
sql: select id, name, nation, goals_scored from player where nation = ? and goals_scored > ? order by name
params: ['NZ', 10]
rows: Ann (NZ, 14) | Cy (NZ, 21)
in memory: Ann | Cy
query: name starts with B or from AU
sql: select id, name, nation, goals_scored from player where (name like ? or nation = ?) order by name
params: ['B%', 'AU']
rows: Bo (NZ, 3) | Dee (AU, 9)
in memory: Bo | Dee
query: from NZ and more than 10 goals, after the rename
sql: select id, name, country_code, goals_scored from player where country_code = ? and goals_scored > ? order by name
params: ['NZ', 10]
rows: Ann (NZ, 14) | Cy (NZ, 21)
in memory: Ann | Cy
```

- Verdict: idiom. `Condition` is an interface, and `AnyOf` is a slice of conditions with its own methods. The parameters are collected through a pointer to a slice.
- The `DataMap` keeps the fields in a slice because the order of a Go map is not fixed, and the column list must have a fixed order.
- The example has no database, so the rows come from the same query object run on the objects in memory.
