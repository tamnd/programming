```go run id=product-record
package main

import (
	"errors"
	"fmt"
	"sort"
	"strings"
)

// row is what the table stores for one product.
type row struct {
	name         string
	price, stock int
}

// products is a stand-in for the products table. The Go standard library
// has database/sql, but no driver for a real database.
var products = struct {
	rows   map[int]row
	nextID int
}{rows: map[int]row{}, nextID: 1}

// Product is an Active Record: one value per row, with the data access and
// the domain logic together.
type Product struct {
	ID           int // 0 means not saved yet
	Name         string
	Price, Stock int
}

// FindProduct and AllProducts are the finders.
func FindProduct(id int) (*Product, bool) {
	r, ok := products.rows[id]
	if !ok {
		return nil, false
	}
	return &Product{id, r.name, r.price, r.stock}, true
}

func AllProducts() []*Product {
	var ids []int
	for id := range products.rows {
		ids = append(ids, id)
	}
	sort.Ints(ids)
	var out []*Product
	for _, id := range ids {
		p, _ := FindProduct(id)
		out = append(out, p)
	}
	return out
}

func (p *Product) Validate() error {
	if p.Stock < 0 {
		return fmt.Errorf("%s: stock cannot be negative", p.Name)
	}
	return nil
}

func (p *Product) Save() error {
	if err := p.Validate(); err != nil {
		return err
	}
	if p.ID == 0 {
		p.ID = products.nextID
		products.nextID++
	}
	products.rows[p.ID] = row{p.Name, p.Price, p.Stock}
	return nil
}

func (p *Product) Delete() { delete(products.rows, p.ID) }

// The domain logic is on the same type.
func (p *Product) NeedsRestock() bool { return p.Stock < 5 }

func (p *Product) Sell(quantity int) (int, error) {
	if quantity > p.Stock {
		return 0, errors.New(p.Name + ": only " + fmt.Sprint(p.Stock) + " in stock")
	}
	p.Stock -= quantity
	return quantity * p.Price, nil
}

func (p *Product) String() string { return fmt.Sprintf("%d %s %d", p.ID, p.Name, p.Stock) }

func main() {
	for _, p := range []Product{{Name: "pen", Price: 120, Stock: 3}, {Name: "ink", Price: 450, Stock: 40}, {Name: "pad", Price: 300, Stock: 0}} {
		if err := p.Save(); err != nil {
			panic(err)
		}
	}

	pen, _ := FindProduct(1)
	total, _ := pen.Sell(2)
	fmt.Println("sold 2 pens for", total)
	pen.Save()
	var restocked []string
	for _, p := range AllProducts() {
		if p.NeedsRestock() {
			p.Stock += 20
			p.Save()
			restocked = append(restocked, p.Name)
		}
	}
	fmt.Println("restocked: " + strings.Join(restocked, ", "))
	var rows []string
	for _, p := range AllProducts() {
		rows = append(rows, p.String())
	}
	fmt.Println("rows: " + strings.Join(rows, " | "))
	ink, _ := FindProduct(2)
	ink.Stock -= 45
	if err := ink.Save(); err != nil {
		fmt.Println("not saved:", err)
	}
	if _, ok := FindProduct(9); !ok {
		fmt.Println("find(9): none")
	}
}
```

```output
sold 2 pens for 240
restocked: pen, pad
rows: 1 pen 21 | 2 ink 40 | 3 pad 20
not saved: ink: stock cannot be negative
find(9): none
```

- Verdict: idiom. The finders are package functions, and `Save` returns an `error`, so a failed validation is a value that the caller checks.
- A map in a package variable stands in for the table. `AllProducts` sorts the ids, because the iteration order of a Go map is not fixed.
