```go run id=insert-then-update
package main

import (
	"fmt"
	"strconv"
)

type row struct{ name, email string }

// A stand-in for the database: a table keyed by id, and a counter like AUTOINCREMENT.
var personTable = map[int64]row{}
var lastRowID int64

// A second table keyed by a meaningful value, the email.
var personByEmail = map[string]string{"cy@example.com": "Cy"}

type Person struct {
	ID    *int64 // The Identity Field: nil until the row exists.
	Name  string
	Email string
}

func (p *Person) showID() string {
	if p.ID == nil {
		return "none"
	}
	return strconv.FormatInt(*p.ID, 10)
}

func save(p *Person) int {
	if p.ID == nil {
		lastRowID++
		personTable[lastRowID] = row{p.Name, p.Email}
		id := lastRowID
		p.ID = &id
		return 1
	}
	if _, ok := personTable[*p.ID]; !ok {
		return 0
	}
	personTable[*p.ID] = row{p.Name, p.Email}
	return 1
}

func find(id int64) *Person {
	r := personTable[id]
	return &Person{ID: &id, Name: r.name, Email: r.email}
}

func main() {
	ann := &Person{Name: "Ann", Email: "ann@example.com"}
	bo := &Person{Name: "Bo", Email: "bo@example.com"}
	fmt.Printf("new: Ann id=%s | Bo id=%s\n", ann.showID(), bo.showID())
	save(ann)
	save(bo)
	fmt.Printf("saved: Ann id=%s | Bo id=%s\n", ann.showID(), bo.showID())

	ann.Email = "ann@new.example"
	rows := save(ann)
	r := personTable[*ann.ID]
	fmt.Printf("update by id: rows %d | %d %s %s\n", rows, *ann.ID, r.name, r.email)

	b := find(2)
	fmt.Printf("find(2): id=%d %s %s\n", *b.ID, b.Name, b.Email)

	first, second := find(1), find(1)
	objects := "different"
	if first == second {
		objects = "same"
	}
	ids := "different"
	if *first.ID == *second.ID {
		ids = "equal"
	}
	fmt.Printf("two loads of row 1: %s objects, %s ids\n", objects, ids)

	// The same change with a meaningful key: the email is the key.
	cy := &Person{Name: "Cy", Email: "cy@example.com"}
	cy.Email = "cy@new.example"
	updated := 0
	if _, ok := personByEmail[cy.Email]; ok {
		personByEmail[cy.Email] = cy.Name
		updated = 1
	}
	fmt.Printf("update by email key: rows %d\n", updated)
}
```

```output
new: Ann id=none | Bo id=none
saved: Ann id=1 | Bo id=2
update by id: rows 1 | 1 Ann ann@new.example
find(2): id=2 Bo bo@example.com
two loads of row 1: different objects, equal ids
update by email key: rows 0
```

- Verdict: idiom. The key is a `*int64`, so `nil` means that the object is not saved yet. A plain `int64` would need 0 as the marker. `==` on two pointers is true only if they point to the same variable, so the two loads of row 1 are different objects.
- With `database/sql`, `Result.LastInsertId` gives the new key for drivers that support it.
