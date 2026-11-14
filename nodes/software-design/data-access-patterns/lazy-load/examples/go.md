```go run id=four-kinds-of-lazy-load
package main

import (
	"fmt"
	"strings"
)

// A stand-in for the database: the tables and a count of SELECT statements.
var customerTable = map[int][2]string{1: {"Ann", "Oslo"}, 2: {"Bo", "Rome"}}
var orderTable = [][2]int{{10, 1}, {11, 2}, {12, 1}}
var selects = 0

func selectCustomer(id int) [2]string {
	selects++
	return customerTable[id]
}

type Customer interface {
	ID() int
	Name() string
	City() string
}

type realCustomer struct {
	id         int
	name, city string
}

func (c *realCustomer) ID() int      { return c.id }
func (c *realCustomer) Name() string { return c.name }
func (c *realCustomer) City() string { return c.city }

func loadCustomer(id int) *realCustomer {
	row := selectCustomer(id)
	return &realCustomer{id, row[0], row[1]}
}

// 1. Lazy initialization: a marker value (nil) means "not loaded".
type Order struct {
	ID, CustomerID int
	customer       Customer
}

func (o *Order) Customer() Customer {
	if o.customer == nil {
		o.customer = loadCustomer(o.CustomerID)
	}
	return o.customer
}

// 2. Virtual proxy: it implements Customer and loads the real one on first use.
type customerProxy struct {
	id   int
	real Customer
}

func (p *customerProxy) load() Customer {
	if p.real == nil {
		p.real = loadCustomer(p.id)
	}
	return p.real
}

func (p *customerProxy) ID() int      { return p.id }
func (p *customerProxy) Name() string { return p.load().Name() }
func (p *customerProxy) City() string { return p.load().City() }

// 3. Value holder: the client asks the holder for the value.
type ValueHolder[T any] struct {
	loader func() T
	loaded bool
	value  T
}

func (h *ValueHolder[T]) GetValue() T {
	if !h.loaded {
		h.value, h.loaded = h.loader(), true
	}
	return h.value
}

// 4. Ghost: the real type, with only the key filled in until first use.
type ghostCustomer struct {
	id         int
	name, city string
	loaded     bool
}

func (g *ghostCustomer) load() {
	if !g.loaded {
		row := selectCustomer(g.id)
		g.name, g.city, g.loaded = row[0], row[1], true
	}
}

func (g *ghostCustomer) ID() int      { return g.id }
func (g *ghostCustomer) Name() string { g.load(); return g.name }
func (g *ghostCustomer) City() string { g.load(); return g.city }

func main() {
	// Each step reads the counter before and after the call that may load.
	selects = 0
	order := &Order{ID: 10, CustomerID: 1}
	before := selects
	name := order.Customer().Name()
	after := selects
	again := order.Customer().Name()
	fmt.Printf("lazy initialization: selects %d | %s | selects %d | %s | selects %d\n", before, name, after, again, selects)

	selects = 0
	var proxy Customer = &customerProxy{id: 2}
	id, before := proxy.ID(), selects
	name, after = proxy.Name(), selects
	city := proxy.City()
	fmt.Printf("virtual proxy: id %d | selects %d | %s | selects %d | %s | selects %d\n", id, before, name, after, city, selects)

	selects = 0
	holder := &ValueHolder[Customer]{loader: func() Customer { return loadCustomer(1) }}
	before = selects
	name = holder.GetValue().Name()
	fmt.Printf("value holder: selects %d | %s | selects %d\n", before, name, selects)

	selects = 0
	var ghost Customer = &ghostCustomer{id: 1}
	id, before = ghost.ID(), selects
	name, city = ghost.Name(), ghost.City()
	fmt.Printf("ghost: id %d | selects %d | %s %s | selects %d\n", id, before, name, city, selects)

	selects = 1 // One SELECT reads the orders.
	var names []string
	for _, row := range orderTable {
		o := &Order{ID: row[0], CustomerID: row[1]}
		names = append(names, o.Customer().Name())
	}
	fmt.Printf("lazy: %s | selects %d\n", strings.Join(names, ", "), selects)

	selects = 1 // One SELECT with a JOIN reads the orders and their customers.
	names = nil
	for _, row := range orderTable {
		names = append(names, customerTable[row[1]][0])
	}
	fmt.Printf("eager: %s | selects %d\n", strings.Join(names, ", "), selects)
}
```

```output
lazy initialization: selects 0 | Ann | selects 1 | Ann | selects 1
virtual proxy: id 2 | selects 0 | Bo | selects 1 | Rome | selects 1
value holder: selects 0 | Ann | selects 1
ghost: id 1 | selects 0 | Ann Oslo | selects 1
lazy: Ann, Bo, Ann | selects 4
eager: Ann, Bo, Ann | selects 1
```

- Verdict: idiom. `Customer` is an interface, and the proxy and the ghost implement it with pointer receivers, because they change their own fields on the first load. `ValueHolder` is generic.
- `sync.OnceValue` gives the same "load once" behaviour and is also safe for concurrent calls. The file reads the counter into variables before each load, so the order of evaluation is explicit.
