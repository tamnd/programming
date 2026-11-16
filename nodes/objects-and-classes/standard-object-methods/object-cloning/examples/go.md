```go run id=assignment-and-slices-clone
package main

import (
	"fmt"
	"slices"
)

type Team struct {
	Name    string
	Members []string
}

// DeepCopy gives the copy its own array of members.
func (t Team) DeepCopy() Team {
	t.Members = slices.Clone(t.Members)
	return t
}

func main() {
	a := Team{Name: "red", Members: []string{"Ann", "Bo"}}
	shallow := a
	deep := a.DeepCopy()

	a.Name = "blue"
	a.Members[0] = "Zoe"
	fmt.Println(shallow.Name, shallow.Members)
	fmt.Println(deep.Name, deep.Members)
}
```

```output for=assignment-and-slices-clone
red [Zoe Bo]
red [Ann Bo]
```

- Verdict: idiom. Assignment copies a struct field by field, so `shallow` has its own `Name`.
- A slice is a small header that points to an array. The copied header points to the same array, so a change of an element shows in both.
- `slices.Clone` makes a new array. The elements are copied by assignment, so a slice of slices or of pointers needs a loop for a deeper copy.
