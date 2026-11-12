```go run id=type-code-to-subclasses
package main

import "fmt"

// Go has no subclasses. Each kind is its own type, and the interface gives the shared methods.
type Employee interface {
	Name() string
	Bonus() int
}

type base struct{ name string }

func (b base) Name() string { return b.name }

type Engineer struct{ base }
type Salesman struct{ base }
type Manager struct{ base }

func (Engineer) Bonus() int { return 1000 }
func (Salesman) Bonus() int { return 3000 }
func (Manager) Bonus() int  { return 5000 }

func createEmployee(name, typeCode string) (Employee, error) {
	switch typeCode {
	case "engineer":
		return Engineer{base{name}}, nil
	case "salesman":
		return Salesman{base{name}}, nil
	case "manager":
		return Manager{base{name}}, nil
	}
	return nil, fmt.Errorf("unknown type %q", typeCode)
}

func main() {
	for _, args := range [][2]string{{"Ana", "salesman"}, {"Bo", "manager"}} {
		e, err := createEmployee(args[0], args[1])
		if err != nil {
			panic(err)
		}
		fmt.Printf("%s %T %d\n", e.Name(), e, e.Bonus())
	}
}
```

```output
Ana main.Salesman 3000
Bo main.Manager 5000
```
