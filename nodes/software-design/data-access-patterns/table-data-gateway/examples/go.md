```go run id=products-gateway
package main

import (
	"fmt"
	"sort"
	"strings"
)

// ProductRow is one row of the products table, as plain data.
type ProductRow struct {
	ID, Price, Stock int
	Name             string
}

// Table is a stand-in for one database table.
type Table struct {
	rows   map[int]ProductRow
	nextID int
}

// ProductGateway holds every operation on the products table.
type ProductGateway struct{ table *Table }

func (g ProductGateway) Find(id int) (ProductRow, bool) {
	r, ok := g.table.rows[id]
	return r, ok
}

func (g ProductGateway) FindAll() []ProductRow {
	out := make([]ProductRow, 0, len(g.table.rows))
	for _, r := range g.table.rows {
		out = append(out, r)
	}
	sort.Slice(out, func(i, j int) bool { return out[i].ID < out[j].ID })
	return out
}

func (g ProductGateway) FindLowStock(limit int) []ProductRow {
	var out []ProductRow
	for _, r := range g.FindAll() {
		if r.Stock < limit {
			out = append(out, r)
		}
	}
	return out
}

func (g ProductGateway) Insert(name string, price, stock int) int {
	id := g.table.nextID
	g.table.nextID++
	g.table.rows[id] = ProductRow{ID: id, Name: name, Price: price, Stock: stock}
	return id
}

func (g ProductGateway) UpdateStock(id, stock int) {
	if r, ok := g.table.rows[id]; ok {
		r.Stock = stock
		g.table.rows[id] = r
	}
}

func (g ProductGateway) Delete(id int) { delete(g.table.rows, id) }

// restock is a transaction script: the rule is here, the data access is in the gateway.
func restock(products ProductGateway, limit, amount int) []string {
	var done []string
	for _, r := range products.FindLowStock(limit) {
		products.UpdateStock(r.ID, r.Stock+amount)
		done = append(done, fmt.Sprintf("%s %d -> %d", r.Name, r.Stock, r.Stock+amount))
	}
	return done
}

func main() {
	products := ProductGateway{&Table{rows: map[int]ProductRow{}, nextID: 1}}
	products.Insert("pen", 120, 3)
	ink := products.Insert("ink", 450, 40)
	products.Insert("pad", 300, 0)
	fmt.Println("restocked: " + strings.Join(restock(products, 5, 20), "; "))
	products.Delete(ink)
	var rows []string
	for _, r := range products.FindAll() {
		rows = append(rows, fmt.Sprintf("%d %s %d %d", r.ID, r.Name, r.Price, r.Stock))
	}
	fmt.Println("rows: " + strings.Join(rows, " | "))
	if _, ok := products.Find(9); !ok {
		fmt.Println("find(9): none")
	}
}
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. The gateway is a struct with methods, and the rows are plain structs that are copied by value.
- The map stands in for the table, because the standard library has no database driver. `FindAll` sorts by id, because the iteration order of a Go map is not fixed.
