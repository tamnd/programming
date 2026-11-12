```go run id=push-down-field
package main

import "fmt"

type Employee struct{ Name string }

type Engineer struct{ Employee }

// Quota is a field of Salesman only.
type Salesman struct {
	Employee
	Quota int
}

func main() {
	fmt.Printf("%+v\n", Engineer{Employee{"Bo"}})
	fmt.Printf("%+v\n", Salesman{Employee{"Ana"}, 50000})
}
```

```output
{Employee:{Name:Bo}}
{Employee:{Name:Ana} Quota:50000}
```
