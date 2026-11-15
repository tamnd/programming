```go run id=two-sessions-one-customer
package main

import (
	"errors"
	"fmt"
	"sync"
)

var errConflict = errors.New("was changed by another session")

type row struct {
	name            string
	credit, version int
}

// Customer is a session's copy of the record, with the version that it read.
type Customer struct {
	id, credit, version int
	name                string
}

func (c Customer) String() string {
	return fmt.Sprintf("%s, credit %d, version %d", c.name, c.credit, c.version)
}

// Table is kept in memory. The mutex makes the check and the update one step,
// like UPDATE ... WHERE version = ? in SQL.
type Table struct {
	mu   sync.Mutex
	rows map[int]row
}

func (t *Table) Load(id int) Customer {
	t.mu.Lock()
	defer t.mu.Unlock()
	r := t.rows[id]
	return Customer{id: id, name: r.name, credit: r.credit, version: r.version}
}

func (t *Table) Save(c *Customer) error {
	t.mu.Lock()
	defer t.mu.Unlock()
	r := t.rows[c.id]
	if r.version != c.version {
		return fmt.Errorf("customer %d %w", c.id, errConflict)
	}
	t.rows[c.id] = row{c.name, c.credit, r.version + 1}
	c.version++
	return nil
}

func trySave(t *Table, who string, c *Customer, what string) bool {
	if err := t.Save(c); err != nil {
		fmt.Printf("%s saves %s: conflict: %v\n", who, what, err)
		return false
	}
	fmt.Printf("%s saves %s: ok, version %d\n", who, what, c.version)
	return true
}

func main() {
	t := &Table{rows: map[int]row{1: {"Acme Ltd", 500, 1}}}

	ann := t.Load(1)
	fmt.Println("ann loads customer 1:", ann)
	bob := t.Load(1)
	fmt.Println("bob loads customer 1:", bob)

	bob.credit = 800
	trySave(t, "bob", &bob, "credit 800")

	ann.name = "Acme Group"
	if !trySave(t, "ann", &ann, "name Acme Group") {
		ann = t.Load(1) // the session reloads, shows the new data, and the user does the edit again
		fmt.Println("ann reloads:", ann)
		ann.name = "Acme Group"
		trySave(t, "ann", &ann, "name Acme Group")
	}

	fmt.Println("final:", t.Load(1))
}
```

```output
ann loads customer 1: Acme Ltd, credit 500, version 1
bob loads customer 1: Acme Ltd, credit 500, version 1
bob saves credit 800: ok, version 2
ann saves name Acme Group: conflict: customer 1 was changed by another session
ann reloads: Acme Ltd, credit 800, version 2
ann saves name Acme Group: ok, version 3
final: Acme Group, credit 800, version 3
```

- Verdict: idiom. A `sync.Mutex` makes the check of the version and the write one step. With `database/sql`, the same check is an `UPDATE` with the version in the `WHERE` clause, and `RowsAffected` tells whether it matched.
- `Save` wraps `errConflict` with `%w`, so a caller can test for a conflict with `errors.Is`.
