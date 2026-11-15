```go run id=customer-and-addresses
package main

import (
	"errors"
	"fmt"
	"strings"
)

var errConflict = errors.New("conflict")

type customer struct {
	name string
	lock string
}

type address struct {
	id       int
	customer int
	street   string
	primary  bool
}

// The tables. A customer and its addresses form one group, with one shared version.
var (
	versions  = map[string]int{"customer 1": 1}
	customers = map[int]customer{1: {name: "Acme Ltd", lock: "customer 1"}}
	addresses = []address{
		{id: 1, customer: 1, street: "1 Elm Street"},
		{id: 2, customer: 1, street: "9 Oak Road"},
	}
	checks = 0
)

// group is a session's copy of the customer and its addresses, with the version of the group.
type group struct {
	customerID int
	lock       string
	version    int
	customer   customer
	addresses  []address
}

func load(customerID int) *group {
	c := customers[customerID]
	g := &group{customerID: customerID, lock: c.lock, version: versions[c.lock], customer: c}
	for _, a := range addresses {
		if a.customer == customerID {
			g.addresses = append(g.addresses, a)
		}
	}
	return g
}

// makePrimary keeps the rule of the group: at most one primary address for each customer.
func (g *group) makePrimary(addressID int) {
	for i := range g.addresses {
		g.addresses[i].primary = g.addresses[i].id == addressID
	}
}

func save(g *group) error {
	checks++ // one check for the whole group, however many members changed
	current := versions[g.lock]
	if current != g.version {
		return fmt.Errorf("%w: the group of customer %d changed (version %d)", errConflict, g.customerID, current)
	}
	versions[g.lock] = current + 1
	g.version = current + 1
	customers[g.customerID] = g.customer
	for _, a := range g.addresses {
		for i := range addresses {
			if addresses[i].id == a.id {
				addresses[i] = a
			}
		}
	}
	return nil
}

func trySave(who string, g *group) {
	if err := save(g); err != nil {
		fmt.Printf("%s saves: %v\n", who, err)
		return
	}
	fmt.Printf("%s saves: ok, group version %d\n", who, g.version)
}

func main() {
	ann, bob := load(1), load(1)
	fmt.Printf("customer 1: %s with %d addresses, group version %d\n", ann.customer.name, len(ann.addresses), ann.version)
	ann.makePrimary(1)
	fmt.Println("ann makes 1 Elm Street the primary address")
	bob.makePrimary(2)
	fmt.Println("bob makes 9 Oak Road the primary address")
	trySave("ann", ann)
	trySave("bob", bob)
	var primary []string
	for _, a := range addresses {
		if a.primary {
			primary = append(primary, a.street)
		}
	}
	fmt.Println("primary addresses in the database:", strings.Join(primary, ", "))
	fmt.Println("version checks:", checks)
}
```

```output
customer 1: Acme Ltd with 2 addresses, group version 1
ann makes 1 Elm Street the primary address
bob makes 9 Oak Road the primary address
ann saves: ok, group version 2
bob saves: conflict: the group of customer 1 changed (version 2)
primary addresses in the database: 1 Elm Street
version checks: 2
```

- Verdict: idiom. `load` copies the customer struct and the address structs into the group, because Go copies a struct value on assignment. `save` returns an error that wraps `errConflict`, so a caller can test it with `errors.Is`.
- The addresses are a slice and not a map, because the order of a range over a map is not defined.
