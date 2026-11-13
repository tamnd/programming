```go run id=product-rows
package main

import (
	"fmt"
	"sort"
	"strings"
)

// record is what the table stores for one row.
type record struct {
	name         string
	price, stock int
}

// Table is a stand-in for one database table.
type Table struct {
	rows   map[int]record
	nextID int
}

// ProductRow is a gateway to one row: the columns are fields, and the row saves itself.
type ProductRow struct {
	table        *Table
	ID           int
	Name         string
	Price, Stock int
}

func (p *ProductRow) Insert() int {
	p.ID = p.table.nextID
	p.table.nextID++
	p.Update()
	return p.ID
}

func (p *ProductRow) Update() { p.table.rows[p.ID] = record{p.Name, p.Price, p.Stock} }

func (p *ProductRow) Delete() { delete(p.table.rows, p.ID) }

func (p *ProductRow) String() string {
	return fmt.Sprintf("%d %s %d %d", p.ID, p.Name, p.Price, p.Stock)
}

// ProductFinder reads rows and makes one ProductRow for each of them.
type ProductFinder struct{ table *Table }

func (f ProductFinder) load(id int, r record) *ProductRow {
	return &ProductRow{f.table, id, r.name, r.price, r.stock}
}

func (f ProductFinder) Find(id int) *ProductRow {
	if r, ok := f.table.rows[id]; ok {
		return f.load(id, r)
	}
	return nil
}

func (f ProductFinder) FindAll() []*ProductRow {
	var out []*ProductRow
	for id, r := range f.table.rows {
		out = append(out, f.load(id, r))
	}
	sort.Slice(out, func(i, j int) bool { return out[i].ID < out[j].ID })
	return out
}

func (f ProductFinder) FindLowStock(limit int) []*ProductRow {
	var out []*ProductRow
	for _, p := range f.FindAll() {
		if p.Stock < limit {
			out = append(out, p)
		}
	}
	return out
}

func restock(finder ProductFinder, limit, amount int) []string {
	var done []string
	for _, p := range finder.FindLowStock(limit) {
		before := p.Stock
		p.Stock += amount
		p.Update()
		done = append(done, fmt.Sprintf("%s %d -> %d", p.Name, before, p.Stock))
	}
	return done
}

func main() {
	table := &Table{rows: map[int]record{}, nextID: 1}
	(&ProductRow{table: table, Name: "pen", Price: 120, Stock: 3}).Insert()
	ink := (&ProductRow{table: table, Name: "ink", Price: 450, Stock: 40}).Insert()
	(&ProductRow{table: table, Name: "pad", Price: 300, Stock: 0}).Insert()
	finder := ProductFinder{table}
	fmt.Println("restocked: " + strings.Join(restock(finder, 5, 20), "; "))
	finder.Find(ink).Delete()
	var rows []string
	for _, p := range finder.FindAll() {
		rows = append(rows, p.String())
	}
	fmt.Println("rows: " + strings.Join(rows, " | "))
	if finder.Find(9) == nil {
		fmt.Println("find(9): none")
	}
}
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. The finder returns pointers, so a caller changes the fields of one gateway and calls `Update` on the same value.
- `FindAll` sorts by id, because the iteration order of a Go map is not fixed.
