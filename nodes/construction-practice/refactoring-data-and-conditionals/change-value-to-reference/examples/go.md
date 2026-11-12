```go run id=one-object-for-one-thing
package main

import "fmt"

type Customer struct {
	ID    string
	Email string
}

// CustomerRepository hands out pointers, so each order refers to one struct.
type CustomerRepository struct {
	byID map[string]*Customer
}

func (r *CustomerRepository) Register(id, email string) *Customer {
	if c, ok := r.byID[id]; ok {
		return c
	}
	c := &Customer{id, email}
	r.byID[id] = c
	return c
}

func (r *CustomerRepository) Get(id string) (*Customer, error) {
	c, ok := r.byID[id]
	if !ok {
		return nil, fmt.Errorf("no customer %s", id)
	}
	return c, nil
}

type Order struct{ Customer *Customer }

func main() {
	repo := &CustomerRepository{byID: map[string]*Customer{}}
	repo.Register("c1", "old@example.org")
	c, err := repo.Get("c1")
	if err != nil {
		panic(err)
	}
	a, b := Order{c}, Order{c}
	a.Customer.Email = "changed@example.org"
	if b.Customer.Email != "changed@example.org" || a.Customer != b.Customer {
		panic("not shared")
	}
	if _, err := repo.Get("c9"); err == nil {
		panic("expected an error")
	}
	fmt.Println("change value to reference: tests pass")
}
```

```output
change value to reference: tests pass
```
