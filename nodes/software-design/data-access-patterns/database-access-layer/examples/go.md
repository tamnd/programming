```go run id=logical-and-physical-layers
package main

import (
	"errors"
	"fmt"
	"sort"
	"strings"
)

// ErrAccess is the error that the application kernel sees, whatever the database says.
var ErrAccess = errors.New("access error")

// ---- Physical access layer: knows the tables. ----

type orderRow struct {
	ID         int
	CustomerID int
	Customer   string // used only in the tuned layout
}

type lineRow struct {
	OrderID int
	Item    string
	Qty     int
}

// Database holds the tables in memory; the example has no database.
type Database struct {
	customers map[int]string
	orders    []orderRow
	lines     []lineRow
}

func (db *Database) Commit() {}

func normalized() *Database {
	return &Database{
		customers: map[int]string{1: "Acme Ltd"},
		orders:    []orderRow{{ID: 10, CustomerID: 1}},
		lines:     []lineRow{{10, "bolt", 2}, {10, "nut", 5}},
	}
}

// After tuning: the customer name is copied into the orders table.
func denormalized() *Database {
	return &Database{
		orders: []orderRow{{ID: 10, Customer: "Acme Ltd"}},
		lines:  []lineRow{{10, "bolt", 2}, {10, "nut", 5}},
	}
}

// OrderView is the protocol of a physical view of orders: read an order, write a line.
type OrderView interface {
	Tables() string
	Read(orderID int) (customer string, lines map[string]int, found bool)
	WriteLine(orderID int, item string, qty int)
	DB() *Database
}

type lineAccess struct{ db *Database }

func (v lineAccess) DB() *Database { return v.db }

func (v lineAccess) lines(orderID int) map[string]int {
	out := map[string]int{}
	for _, r := range v.db.lines {
		if r.OrderID == orderID {
			out[r.Item] = r.Qty
		}
	}
	return out
}

func (v lineAccess) WriteLine(orderID int, item string, qty int) {
	for i, r := range v.db.lines {
		if r.OrderID == orderID && r.Item == item {
			v.db.lines[i].Qty = qty
		}
	}
}

// NormalizedOrderView reads an order from the normalized tables.
type NormalizedOrderView struct{ lineAccess }

func (NormalizedOrderView) Tables() string { return "customer, orders, order_line" }

func (v NormalizedOrderView) Read(orderID int) (string, map[string]int, bool) {
	for _, o := range v.db.orders {
		if o.ID == orderID {
			if name, ok := v.db.customers[o.CustomerID]; ok {
				return name, v.lines(orderID), true
			}
		}
	}
	return "", nil, false
}

// DenormalizedOrderView reads an order from the tuned tables: one table less to read.
type DenormalizedOrderView struct{ lineAccess }

func (DenormalizedOrderView) Tables() string { return "orders, order_line" }

func (v DenormalizedOrderView) Read(orderID int) (string, map[string]int, bool) {
	for _, o := range v.db.orders {
		if o.ID == orderID {
			return o.Customer, v.lines(orderID), true
		}
	}
	return "", nil, false
}

// ---- Logical access layer: the interface of the application kernel. ----

// Order is the logical view of an order. It knows nothing of tables.
type Order struct {
	ID       int
	Customer string
	Lines    map[string]int
	changed  map[string]bool
}

func (o *Order) SetQty(item string, qty int) {
	o.Lines[item] = qty
	o.changed[item] = true
}

type AccessLayer struct {
	physical OrderView
	cache    map[int]*Order
	order    []int
	reads    int
}

func (l *AccessLayer) Start() {
	l.cache, l.order, l.reads = map[int]*Order{}, nil, 0
}

func (l *AccessLayer) GetOrder(orderID int) (*Order, error) {
	if o, ok := l.cache[orderID]; ok {
		return o, nil
	}
	customer, lines, found := l.physical.Read(orderID)
	l.reads++
	if !found {
		return nil, fmt.Errorf("%w: no order %d", ErrAccess, orderID)
	}
	o := &Order{orderID, customer, lines, map[string]bool{}}
	l.cache[orderID] = o
	l.order = append(l.order, orderID)
	return o, nil
}

func (l *AccessLayer) Commit() []string {
	var written []string
	for _, id := range l.order {
		o := l.cache[id]
		for _, item := range sortedKeys(o.changed) {
			l.physical.WriteLine(o.ID, item, o.Lines[item])
			written = append(written, fmt.Sprintf("order %d %s x %d", o.ID, item, o.Lines[item]))
		}
		o.changed = map[string]bool{}
	}
	l.physical.DB().Commit()
	return written
}

func sortedKeys[V any](m map[string]V) []string {
	var keys []string
	for k := range m {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	return keys
}

// ---- Application kernel: uses only the logical access layer. ----

func invoice(l *AccessLayer, orderID int) (string, error) {
	o, err := l.GetOrder(orderID)
	if err != nil {
		return "", err
	}
	parts := []string{o.Customer}
	for _, item := range sortedKeys(o.Lines) {
		parts = append(parts, fmt.Sprintf("%s x %d", item, o.Lines[item]))
	}
	return strings.Join(parts, " | "), nil
}

func run(name string, view OrderView) {
	l := &AccessLayer{physical: view}
	fmt.Println("physical layout:", name)
	l.Start()
	s, _ := invoice(l, 10)
	fmt.Println("invoice 10:", s)
	s, _ = invoice(l, 10)
	fmt.Println("invoice 10:", s)
	fmt.Printf("physical reads: %d, tables %s\n", l.reads, view.Tables())
	o, _ := l.GetOrder(10)
	o.SetQty("bolt", 3)
	fmt.Println("commit:", strings.Join(l.Commit(), ", "))
	l.Start()
	s, _ = invoice(l, 10)
	fmt.Println("invoice 10 in a new transaction:", s)
	if _, err := invoice(l, 99); errors.Is(err, ErrAccess) {
		fmt.Println("error:", strings.TrimPrefix(err.Error(), "access error: "))
	}
}

func main() {
	run("normalized", NormalizedOrderView{lineAccess{normalized()}})
	run("denormalized", DenormalizedOrderView{lineAccess{denormalized()}})
}
```

```output
physical layout: normalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables customer, orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
physical layout: denormalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
```

- Verdict: idiom. `OrderView` is an interface, and both views embed `lineAccess` for the shared methods. The logical layer wraps the error with `%w`, so the kernel tests it with `errors.Is`.
- The cache keeps a slice of keys next to the map, because the order of a Go map is not fixed and the commit must write in a fixed order.
