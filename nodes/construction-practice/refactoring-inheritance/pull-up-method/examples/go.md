```go run id=pull-up
package main

import "fmt"

// Go has no subclasses. The shared method goes on a struct that the others embed.
type Employee struct {
	Name        string
	MonthlyCost int
}

func (e Employee) AnnualCost() int { return e.MonthlyCost * 12 }

type Salesman struct{ Employee }
type Engineer struct {
	Employee
	Level int
}

func main() {
	ana := Salesman{Employee{"Ana", 1000}}
	bo := Engineer{Employee{"Bo", 1500}, 2}
	// AnnualCost is promoted from the embedded Employee.
	fmt.Println(ana.AnnualCost(), bo.AnnualCost())
}
```

```output
12000 18000
```
