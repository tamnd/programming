```go run id=pull-up-constructor
package main

import "fmt"

type Party struct{ Name string }

// newParty sets up the shared part once.
func newParty(name string) Party { return Party{Name: name} }

type Employee struct {
	Party
	ID          string
	MonthlyCost int
}

func NewEmployee(name, id string, monthlyCost int) Employee {
	return Employee{Party: newParty(name), ID: id, MonthlyCost: monthlyCost}
}

type Department struct {
	Party
	Staff []Employee
}

func NewDepartment(name string, staff []Employee) Department {
	return Department{Party: newParty(name), Staff: staff}
}

func main() {
	ana := NewEmployee("Ana", "E1", 1000)
	sales := NewDepartment("Sales", []Employee{ana})
	fmt.Println(ana.Name, sales.Name)
}
```

```output
Ana Sales
```
