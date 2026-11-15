```go run id=assembler-and-wire
package main

import (
	"encoding/json"
	"fmt"
	"strings"
)

func cents(n int) string { return fmt.Sprintf("%d.%02d", n/100, n%100) }

// Domain objects on the server. Some data must never leave the server.
type Line struct {
	item                  string
	qty, price, costPrice int
}

type Order struct {
	number int
	lines  []Line
}

func (o Order) Total() int {
	t := 0
	for _, l := range o.lines {
		t += l.qty * l.price
	}
	return t
}

type Customer struct {
	name, email, passwordHash string
	orders                    []Order
}

// OrderSummaryDTO and CustomerDTO are the data transfer objects: only data,
// shaped for one remote call. The struct tags give their form on the wire.
type OrderSummaryDTO struct {
	Number int `json:"number"`
	Total  int `json:"total"`
}

type CustomerDTO struct {
	Name   string            `json:"name"`
	Email  string            `json:"email"`
	Orders []OrderSummaryDTO `json:"orders"`
}

func (d CustomerDTO) ToWire() string {
	b, _ := json.Marshal(d)
	return string(b)
}

func FromWire(text string) CustomerDTO {
	var d CustomerDTO
	_ = json.Unmarshal([]byte(text), &d)
	return d
}

// The assembler moves the data between the domain objects and the DTOs, on the server.
func ToDTO(c *Customer) CustomerDTO {
	d := CustomerDTO{Name: c.name, Email: c.email}
	for _, o := range c.orders {
		d.Orders = append(d.Orders, OrderSummaryDTO{o.number, o.Total()})
	}
	return d
}

func UpdateFrom(d CustomerDTO, c *Customer) {
	c.name, c.email = d.Name, d.Email // the orders are read only
}

func main() {
	ann := &Customer{"Ann", "ann@example.com", "pbkdf2:9f86d081", []Order{
		{7, []Line{{"lamp", 2, 2500, 1400}, {"desk", 1, 7500, 5100}, {"pen", 4, 500, 90}}},
		{8, []Line{{"chair", 1, 4000, 2600}}},
	}}

	// Server: one call returns all the data that the screen needs.
	wire := ToDTO(ann).ToWire()
	leak := "no"
	if strings.Contains(wire, ann.passwordHash) {
		leak = "yes"
	}
	fmt.Println("password hash on the wire:", leak)

	// Client: it reads the DTO, and changes its own copy.
	dto := FromWire(wire)
	fmt.Printf("client: %s <%s>, %d orders\n", dto.Name, dto.Email, len(dto.Orders))
	for _, o := range dto.Orders {
		fmt.Printf("  order %d: %s\n", o.Number, cents(o.Total))
	}
	dto.Email = "ann@example.org"
	fmt.Println("server after a change on the client:", ann.email)

	// Client sends the DTO back; the assembler updates the domain object.
	UpdateFrom(FromWire(dto.ToWire()), ann)
	fmt.Println("server after the update call:", ann.email)
}
```

```output
password hash on the wire: no
client: Ann <ann@example.com>, 2 orders
  order 7: 145.00
  order 8: 40.00
server after a change on the client: ann@example.com
server after the update call: ann@example.org
```

- Verdict: idiom. The DTOs are structs with exported fields and `json` struct tags, and `encoding/json` serializes them. The domain structs have only fields that are not exported, so `json.Marshal` of a `Customer` would give `{}`, and the password hash could not leak.
- `ToDTO` and `UpdateFrom` are the assembler. `UpdateFrom` copies only the name and the email.
