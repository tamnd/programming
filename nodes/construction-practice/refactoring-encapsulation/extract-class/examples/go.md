```go run id=two-jobs
package main

import (
	"fmt"
	"strings"
)

// The address job in its own type.
type Address struct {
	Street, City, Postcode string
}

func (a Address) Label() string {
	return a.Street + "\n" + a.Postcode + " " + strings.ToUpper(a.City)
}

// The order embeds the address. Embedding promotes Label, so the
// old calls o.Label() still compile; o.Address names the part.
type Order struct {
	ID int
	Address
}

func main() {
	o := Order{ID: 1, Address: Address{"Main St 5", "Berlin", "10115"}}
	fmt.Println(o.Label())
	if o.Address != (Address{"Main St 5", "Berlin", "10115"}) {
		panic("address changed") // structs with comparable fields compare by value
	}
	fmt.Println("extract class: tests pass")
}
```

```output
Main St 5
10115 BERLIN
extract class: tests pass
```
