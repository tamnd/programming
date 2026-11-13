```go run id=product-mapper
package main

import (
	"errors"
	"fmt"
	"slices"
	"sort"
	"strings"
)

// Product is a domain object. It has no storage code and no idea that a database exists.
type Product struct {
	ID           int // 0 means not stored yet
	Name         string
	Price, Stock int
	Tags         []string
}

func (p *Product) Sell(quantity int) (int, error) {
	if quantity > p.Stock {
		return 0, errors.New(p.Name + ": only " + fmt.Sprint(p.Stock) + " in stock")
	}
	p.Stock -= quantity
	return quantity * p.Price, nil
}

func (p *Product) String() string {
	return fmt.Sprintf("%d %s %d %d [%s]", p.ID, p.Name, p.Price, p.Stock, strings.Join(p.Tags, ", "))
}

// A stand-in for the database. The Go standard library has database/sql,
// but no driver for a real database. The two tables have their own column names.
type productRow struct {
	title           string
	priceCents, qty int
}

type tagRow struct {
	productID int
	tag       string
}

type Database struct {
	products    map[int]productRow
	productTags []tagRow
	nextID      int
}

// ProductMapper moves products between objects and the two tables.
type ProductMapper struct{ db *Database }

func (m ProductMapper) Find(id int) (*Product, bool) {
	row, ok := m.db.products[id]
	if !ok {
		return nil, false
	}
	var tags []string
	for _, t := range m.db.productTags {
		if t.productID == id {
			tags = append(tags, t.tag)
		}
	}
	sort.Strings(tags)
	return &Product{id, row.title, row.priceCents, row.qty, tags}, true
}

func (m ProductMapper) Insert(p *Product) {
	p.ID = m.db.nextID
	m.db.nextID++
	m.Update(p)
}

func (m ProductMapper) Update(p *Product) {
	m.db.products[p.ID] = productRow{p.Name, p.Price, p.Stock}
	m.deleteTags(p.ID)
	for _, tag := range p.Tags {
		m.db.productTags = append(m.db.productTags, tagRow{p.ID, tag})
	}
}

func (m ProductMapper) Delete(p *Product) {
	m.deleteTags(p.ID)
	delete(m.db.products, p.ID)
}

func (m ProductMapper) deleteTags(id int) {
	m.db.productTags = slices.DeleteFunc(m.db.productTags, func(t tagRow) bool { return t.productID == id })
}

func main() {
	db := &Database{products: map[int]productRow{}, nextID: 1}
	mapper := ProductMapper{db}
	mapper.Insert(&Product{Name: "pen", Price: 120, Stock: 3, Tags: []string{"office"}})
	ink := &Product{Name: "ink", Price: 450, Stock: 40, Tags: []string{"office"}}
	mapper.Insert(ink)

	pen, _ := mapper.Find(1)
	total, _ := pen.Sell(2)
	fmt.Println("sold:", total)
	pen.Tags = append(pen.Tags, "sale")
	mapper.Update(pen)
	stored, _ := mapper.Find(1)
	fmt.Println("object:", stored)
	var ids []int
	for id := range db.products {
		ids = append(ids, id)
	}
	sort.Ints(ids)
	var rows []string
	for _, id := range ids {
		r := db.products[id]
		rows = append(rows, fmt.Sprintf("%d %s %d %d", id, r.title, r.priceCents, r.qty))
	}
	fmt.Println("products: " + strings.Join(rows, " | "))
	var tagRows []string
	for _, t := range db.productTags {
		tagRows = append(tagRows, fmt.Sprintf("%d %s", t.productID, t.tag))
	}
	sort.Strings(tagRows)
	fmt.Println("product_tags: " + strings.Join(tagRows, " | "))
	mapper.Delete(ink)
	fmt.Printf("after delete: %d product, %d tags\n", len(db.products), len(db.productTags))
	if _, ok := mapper.Find(2); !ok {
		fmt.Println("find(2): none")
	}
}
```

```output
sold: 240
object: 1 pen 120 1 [office, sale]
products: 1 pen 120 1 | 2 ink 450 40
product_tags: 1 office | 1 sale | 2 office
after delete: 1 product, 2 tags
find(2): none
```

- Verdict: idiom. `Product` is a plain struct, and `ProductMapper` is the only type that knows the row types.
- `slices.DeleteFunc` removes the old tag rows before `Update` writes the new ones. The ids are sorted for printing, because the iteration order of a Go map is not fixed.
