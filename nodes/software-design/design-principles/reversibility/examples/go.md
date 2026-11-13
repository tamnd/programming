```go run id=reversible-migration
package main

import (
	"fmt"
	"slices"
	"strings"
)

type Table struct{ Columns []string }

type Migration struct{ Up, Down func(*Table) }

func main() {
	table := &Table{Columns: []string{"id", "name"}}
	addEmail := Migration{
		Up: func(t *Table) { t.Columns = append(t.Columns, "email") },
		Down: func(t *Table) {
			t.Columns = slices.DeleteFunc(t.Columns, func(c string) bool { return c == "email" })
		},
	}

	fmt.Printf("before: %s\n", strings.Join(table.Columns, ", "))
	addEmail.Up(table)
	fmt.Printf("up:     %s\n", strings.Join(table.Columns, ", "))
	addEmail.Down(table) // the decision is reversed
	fmt.Printf("down:   %s\n", strings.Join(table.Columns, ", "))
}
```

```output
before: id, name
up:     id, name, email
down:   id, name
```
