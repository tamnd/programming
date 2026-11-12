```go run id=push-down
package main

import "fmt"

type Employee struct{ Name string }

type Engineer struct{ Employee }

// Quota is a method of Salesman only, not of the embedded Employee.
type Salesman struct {
	Employee
	quota int
}

func (s Salesman) Quota() int { return s.quota }

func main() {
	ana := Salesman{Employee{"Ana"}, 50000}
	_ = Engineer{Employee{"Bo"}} // Engineer has no Quota method.
	fmt.Println(ana.Name, ana.Quota())
}
```

```output
Ana 50000
```
