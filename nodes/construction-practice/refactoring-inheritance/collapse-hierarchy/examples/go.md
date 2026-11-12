```go run id=collapse
package main

import "fmt"

// Salesman, a struct that embedded Employee to add Region, is gone.
type Employee struct {
	Name   string
	Region string
}

func NewEmployee(name string) Employee { return Employee{Name: name, Region: "none"} }

func main() {
	ana := NewEmployee("Ana")
	ana.Region = "north"
	fmt.Println(ana.Name, ana.Region)
}
```

```output
Ana north
```
