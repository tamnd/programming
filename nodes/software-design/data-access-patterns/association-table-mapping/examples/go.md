```go run id=link-table
package main

import (
	"errors"
	"fmt"
	"slices"
	"sort"
	"strings"
)

type link struct{ employeeID, skillID int64 }

// A stand-in for the database: two entity tables and a link table of key pairs.
var (
	employeeTable = map[int64]string{}
	skillTable    = map[int64]string{1: "Python", 2: "SQL", 3: "Go"}
	employeeSkill = map[link]bool{} // The pair is the key, like a two-column primary key.
	lastEmployee  int64
)

func insertLink(employeeID, skillID int64) error {
	l := link{employeeID, skillID}
	if employeeSkill[l] {
		return errors.New("UNIQUE constraint failed: employee_skill.employee_id, employee_skill.skill_id")
	}
	employeeSkill[l] = true
	return nil
}

func linkRows() []link {
	rows := make([]link, 0, len(employeeSkill))
	for l := range employeeSkill {
		rows = append(rows, l)
	}
	sort.Slice(rows, func(i, j int) bool {
		if rows[i].employeeID != rows[j].employeeID {
			return rows[i].employeeID < rows[j].employeeID
		}
		return rows[i].skillID < rows[j].skillID
	})
	return rows
}

type Employee struct {
	ID     int64
	Name   string
	Skills []int64 // A list of skill keys.
}

func insert(e *Employee) error {
	lastEmployee++
	employeeTable[lastEmployee] = e.Name
	e.ID = lastEmployee
	for _, s := range e.Skills {
		if err := insertLink(e.ID, s); err != nil {
			return err
		}
	}
	return nil
}

func updateSkills(e *Employee) (deleted, inserted int, err error) {
	// Compare the collection with the link rows: insert the new pairs, delete the old ones.
	var inDB []int64
	for _, l := range linkRows() {
		if l.employeeID == e.ID {
			inDB = append(inDB, l.skillID)
		}
	}
	for _, s := range e.Skills {
		if !slices.Contains(inDB, s) {
			if err := insertLink(e.ID, s); err != nil {
				return deleted, inserted, err
			}
			inserted++
		}
	}
	for _, s := range inDB {
		if !slices.Contains(e.Skills, s) {
			delete(employeeSkill, link{e.ID, s})
			deleted++
		}
	}
	return deleted, inserted, nil
}

func skillsOf(employeeID int64) string {
	var names []string
	for _, l := range linkRows() {
		if l.employeeID == employeeID {
			names = append(names, skillTable[l.skillID])
		}
	}
	return strings.Join(names, ", ")
}

func employeesWith(skillID int64) string {
	var names []string
	for _, l := range linkRows() {
		if l.skillID == skillID {
			names = append(names, employeeTable[l.employeeID])
		}
	}
	return strings.Join(names, ", ")
}

func links() string {
	var parts []string
	for _, l := range linkRows() {
		parts = append(parts, fmt.Sprintf("%d-%d", l.employeeID, l.skillID))
	}
	return strings.Join(parts, " ")
}

func main() {
	ann := &Employee{Name: "Ann", Skills: []int64{1, 2}}
	if err := insert(ann); err != nil {
		fmt.Println(err)
	}
	if err := insert(&Employee{Name: "Bo", Skills: []int64{1}}); err != nil {
		fmt.Println(err)
	}
	fmt.Println("links:", links())
	fmt.Println("Ann:", skillsOf(1))
	fmt.Println("Python:", employeesWith(1))

	ann.Skills = []int64{2, 3}
	deleted, inserted, err := updateSkills(ann)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Printf("update Ann: deleted %d, inserted %d\n", deleted, inserted)
	fmt.Println("links:", links())
	fmt.Println("Ann:", skillsOf(1))

	if err := insertLink(2, 1); err != nil {
		fmt.Println("link 2-1 again:", err)
	}
}
```

```output
links: 1-1 1-2 2-1
Ann: Python, SQL
Python: Ann, Bo
update Ann: deleted 1, inserted 1
links: 1-2 1-3 2-1
Ann: SQL, Go
link 2-1 again: UNIQUE constraint failed: employee_skill.employee_id, employee_skill.skill_id
```

- Verdict: idiom. A link row is a struct of two keys, and the link table is a `map[link]bool`, so the pair is the key of the map. Go maps have no order, so `linkRows` sorts the pairs before it prints them.
- `insertLink` and `updateSkills` return an `error` for a duplicate pair, as a database driver returns the error of the constraint.
