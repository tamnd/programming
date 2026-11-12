```go run id=factory-function
package main

import "fmt"

var legalTypeCodes = map[byte]string{'E': "Engineer", 'M': "Manager", 'S': "Salesperson"}

type Employee struct {
	name     string
	typeCode byte
}

func NewEmployee(name string, typeCode byte) Employee {
	return Employee{name: name, typeCode: typeCode}
}

func NewEngineer(name string) Employee {
	return NewEmployee(name, 'E')
}

func (e Employee) Type() string { return legalTypeCodes[e.typeCode] }

func main() {
	before := NewEmployee("Ana", 'E')
	after := NewEngineer("Ana")
	if after != before {
		panic("mismatch")
	}
	fmt.Printf("%s: %s\n", after.name, after.Type())
}
```

```output
Ana: Engineer
```
