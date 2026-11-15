```go run id=domain-object-supertype
package main

import (
	"fmt"
	"strings"
)

// Go has no inheritance. The layer supertype is a struct that each domain type
// embeds, and a function that works on an interface of all domain objects.

type rule struct {
	ok      bool
	message string
}

// DomainObject is embedded in every domain type of the layer.
type DomainObject struct {
	Kind string
	ID   int
}

// Key is the identity of a domain object: its kind and its ID.
func (d DomainObject) Key() DomainObject { return d }

// Entity is what every domain type has, through the embedded DomainObject
// and its own Name and Rules methods.
type Entity interface {
	Key() DomainObject
	Name() string
	Rules() []rule
}

// Check is the shared behaviour of the layer.
func Check(e Entity) string {
	var problems []string
	for _, r := range e.Rules() {
		if !r.ok {
			problems = append(problems, r.message)
		}
	}
	if len(problems) == 0 {
		return "valid"
	}
	return "invalid: " + strings.Join(problems, ", ")
}

// Same compares two domain objects by kind and ID.
func Same(a, b Entity) bool { return a.Key() == b.Key() }

type Customer struct {
	DomainObject
	name string
}

func NewCustomer(id int, name string) Customer { return Customer{DomainObject{"customer", id}, name} }
func (c Customer) Name() string                { return c.name }
func (c Customer) Rules() []rule               { return []rule{{c.name != "", "name is empty"}} }

type Product struct {
	DomainObject
	name       string
	priceCents int
}

func NewProduct(id int, name string, price int) Product {
	return Product{DomainObject{"product", id}, name, price}
}
func (p Product) Name() string { return p.name }
func (p Product) Rules() []rule {
	return []rule{{p.name != "", "name is empty"}, {p.priceCents >= 0, "price is negative"}}
}

func yesNo(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	for _, e := range []Entity{NewCustomer(1, "ann"), NewProduct(2, "pen", 150), NewProduct(3, "", -5)} {
		name := e.Name()
		if name == "" {
			name = "(no name)"
		}
		fmt.Printf("%s %d %s: %s\n", e.Key().Kind, e.Key().ID, name, Check(e))
	}
	fmt.Println("customer 1 equals customer 1 loaded again:", yesNo(Same(NewCustomer(1, "ann"), NewCustomer(1, "ann"))))
	fmt.Println("customer 1 equals product 1:", yesNo(Same(NewCustomer(1, "ann"), NewProduct(1, "ann", 0))))
}
```

```output
customer 1 ann: valid
product 2 pen: valid
product 3 (no name): invalid: name is empty, price is negative
customer 1 equals customer 1 loaded again: yes
customer 1 equals product 1: no
```

- Verdict: idiom. Go has no inheritance. Each domain type embeds the `DomainObject` struct, so it gets the fields `Kind` and `ID` and the method `Key`. The shared behaviour is the functions `Check` and `Same`, which take the `Entity` interface.
- An embedded struct cannot call a method of the type that embeds it, so the shared code is a function on the interface, not a method of `DomainObject`.
