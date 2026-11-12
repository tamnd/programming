```go run id=subclass-to-field
package main

import "fmt"

type Person struct {
	Name       string
	GenderCode string
}

func NewPerson(name, genderCode string) Person {
	if genderCode == "" {
		genderCode = "X"
	}
	return Person{Name: name, GenderCode: genderCode}
}

func (p Person) IsMale() bool { return p.GenderCode == "M" }

func main() {
	for _, p := range []Person{NewPerson("Ana", "F"), NewPerson("Bo", "M"), NewPerson("Cy", "")} {
		fmt.Println(p.Name, p.GenderCode, p.IsMale())
	}
}
```

```output
Ana F false
Bo M true
Cy X false
```
