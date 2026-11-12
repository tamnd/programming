```go run id=no-setter
package main

import "fmt"

type PersonBefore struct{ id string }

func (p *PersonBefore) ID() string      { return p.id }
func (p *PersonBefore) SetID(id string) { p.id = id }

// Person has no SetID. Code in other packages cannot assign to the unexported field id.
type Person struct{ id string }

func NewPerson(id string) Person { return Person{id: id} }

func (p Person) ID() string { return p.id }

func main() {
	var before PersonBefore
	before.SetID("1234")
	before.SetID("9999")
	if before.ID() != "9999" {
		panic("mismatch")
	}
	ana := NewPerson("1234")
	fmt.Println(ana.ID())
}
```

```output
1234
```
