```go run id=pull-up-field
package main

import "fmt"

// The shared field is declared once, in a struct that both types embed.
type Employee struct{ Name string }

type Salesman struct{ Employee }
type Engineer struct{ Employee }

func main() {
	ana := Salesman{Employee{Name: "Ana"}}
	bo := Engineer{Employee{Name: "Bo"}}
	// Name is promoted from the embedded Employee.
	fmt.Println([]string{ana.Name, bo.Name})
}
```

```output
[Ana Bo]
```
