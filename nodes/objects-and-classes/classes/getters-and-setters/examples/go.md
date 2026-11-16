```go run id=owner-and-set-owner
package main

import (
	"errors"
	"fmt"
	"strings"
)

// Employee keeps its fields unexported, so other packages use the methods.
type Employee struct {
	name  string
	email string
}

func NewEmployee(name, email string) (*Employee, error) {
	e := &Employee{name: name}
	if err := e.SetEmail(email); err != nil {
		return nil, err
	}
	return e, nil
}

// Name is the getter of name. It is not called GetName.
func (e *Employee) Name() string { return e.name }

// Email is the getter of email.
func (e *Employee) Email() string { return e.email }

// SetEmail cleans and checks the address. It returns an error instead of a panic.
func (e *Employee) SetEmail(email string) error {
	clean := strings.ToLower(strings.TrimSpace(email))
	if !strings.Contains(clean, "@") {
		return errors.New("not an email address: " + clean)
	}
	e.email = clean
	return nil
}

func main() {
	e, _ := NewEmployee("Ana", "ana@example.com")
	_ = e.SetEmail("  Ana@Example.COM ")
	fmt.Println(e.Name(), e.Email())
	if err := e.SetEmail("not an address"); err != nil {
		fmt.Println(err)
	}
	fmt.Println(e.Email())
}
```

```output for=owner-and-set-owner
Ana ana@example.com
not an email address: not an address
ana@example.com
```

The getter has the name of the field with a capital letter, `Email`, and the setter is `SetEmail`. The fields start with a lower-case letter, so code in other packages cannot read or set them directly. Code in the same package still can, because Go controls access per package, not per type. The setter returns an `error`, which is the usual way in Go to report a bad value.
