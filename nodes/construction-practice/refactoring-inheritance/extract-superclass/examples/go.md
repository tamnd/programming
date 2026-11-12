```go run id=extract-superclass
package main

import "fmt"

// Party holds the shared field. Both types embed it.
type Party struct{ Name string }

// Coster is the shared method set.
type Coster interface{ AnnualCost() int }

type Employee struct {
	Party
	ID          string
	MonthlyCost int
}

func (e Employee) AnnualCost() int { return e.MonthlyCost * 12 }

type Department struct {
	Party
	Staff []Employee
}

func (d Department) AnnualCost() int {
	total := 0
	for _, e := range d.Staff {
		total += e.AnnualCost()
	}
	return total
}

func main() {
	staff := []Employee{{Party{"Ana"}, "E1", 1000}, {Party{"Bo"}, "E2", 1500}}
	sales := Department{Party{"Sales"}, staff}
	for _, c := range []Coster{staff[0], staff[1], sales} {
		fmt.Println(c.AnnualCost())
	}
	fmt.Println(sales.Name)
}
```

```output
12000
18000
30000
Sales
```
