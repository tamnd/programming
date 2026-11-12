```go run id=query-not-parameter
package main

import "fmt"

type Employee struct {
	Name  string
	grade int
}

func (e Employee) Grade() int { return e.grade }

func availableVacationBefore(e Employee, grade int) int {
	return 20 + 2*grade
}

func availableVacation(e Employee) int {
	return 20 + 2*e.Grade()
}

func main() {
	ana := Employee{Name: "Ana", grade: 3}
	if availableVacation(ana) != availableVacationBefore(ana, ana.Grade()) || availableVacationBefore(ana, 5) == availableVacation(ana) {
		panic("mismatch")
	}
	fmt.Println(availableVacation(ana))
}
```

```output
26
```
