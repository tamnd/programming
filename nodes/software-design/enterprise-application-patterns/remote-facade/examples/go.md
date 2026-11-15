```go run id=one-call-for-an-order
package main

import (
	"encoding/json"
	"fmt"
)

func cents(n int) string { return fmt.Sprintf("%d.%02d", n/100, n%100) }

// Fine-grained domain objects: small objects with small methods.
type Address struct{ street, city string }

func (a *Address) Street() string             { return a.street }
func (a *Address) City() string               { return a.city }
func (a *Address) MoveTo(street, city string) { a.street, a.city = street, city }

type Customer struct {
	name    string
	address *Address
}

func (c *Customer) Name() string      { return c.name }
func (c *Customer) Address() *Address { return c.address }

type Line struct {
	item       string
	qty, price int
}

func (l Line) Amount() int { return l.qty * l.price }

type Order struct {
	number   int
	customer *Customer
	lines    []Line
}

func (o *Order) Total() int {
	t := 0
	for _, l := range o.lines {
		t += l.Amount()
	}
	return t
}

// The data that crosses the network.
type LineView struct {
	Item  string `json:"item"`
	Qty   int    `json:"qty"`
	Price int    `json:"price"`
}

type OrderView struct {
	Number   int        `json:"number"`
	Customer string     `json:"customer"`
	ShipTo   string     `json:"ship_to"`
	Lines    []LineView `json:"lines"`
	Total    int        `json:"total"`
}

// OrderFacade is the remote facade: coarse-grained methods over the fine-grained
// objects, with no domain logic.
type OrderFacade struct{ orders map[int]*Order }

func (f *OrderFacade) GetOrder(number int) OrderView {
	o := f.orders[number]
	a := o.customer.Address()
	v := OrderView{Number: o.number, Customer: o.customer.Name(), ShipTo: a.Street() + ", " + a.City(), Total: o.Total()}
	for _, l := range o.lines {
		v.Lines = append(v.Lines, LineView{l.item, l.qty, l.price})
	}
	return v
}

func (f *OrderFacade) ChangeAddress(number int, street, city string) {
	f.orders[number].customer.Address().MoveTo(street, city)
}

// Network counts each call, and only data crosses it, as JSON text.
type Network struct {
	server *OrderFacade
	calls  int
}

func (n *Network) GetOrder(number int) OrderView {
	n.calls++
	reply, _ := json.Marshal(n.server.GetOrder(number))
	var v OrderView
	_ = json.Unmarshal(reply, &v)
	return v
}

func (n *Network) ChangeAddress(number int, street, city string) {
	n.calls++
	n.server.ChangeAddress(number, street, city)
}

func main() {
	ann := &Customer{"Ann", &Address{"1 Elm Street", "Leeds"}}
	orders := map[int]*Order{7: {7, ann, []Line{{"lamp", 2, 2500}, {"desk", 1, 7500}, {"pen", 4, 500}}}}
	net := &Network{server: &OrderFacade{orders}}

	// The client: one remote call gets all the data that the screen needs.
	view := net.GetOrder(7)
	fmt.Printf("order %d for %s: %d lines, total %s\n", view.Number, view.Customer, len(view.Lines), cents(view.Total))
	for _, l := range view.Lines {
		fmt.Printf("  %s x%d at %s\n", l.Item, l.Qty, cents(l.Price))
	}
	fmt.Println("ship to:", view.ShipTo)

	net.ChangeAddress(7, "2 Oak Road", "York")
	view.ShipTo = "not sent back" // the client has a copy; this changes nothing on the server
	fmt.Println("ship to:", net.GetOrder(7).ShipTo)
	fmt.Println("remote calls:", net.calls)
}
```

```output
order 7 for Ann: 3 lines, total 145.00
  lamp x2 at 25.00
  desk x1 at 75.00
  pen x4 at 5.00
ship to: 1 Elm Street, Leeds
ship to: 2 Oak Road, York
remote calls: 3
```

- Verdict: idiom. The facade returns an `OrderView` struct, and the struct tags give its form in JSON. The fields of the domain structs are not exported, so `encoding/json` cannot serialize the domain objects by mistake.
- `Network` has one method for each method of the facade. Each method counts the call, and `GetOrder` sends the reply through `json.Marshal` and `json.Unmarshal`, so the client gets a copy.
