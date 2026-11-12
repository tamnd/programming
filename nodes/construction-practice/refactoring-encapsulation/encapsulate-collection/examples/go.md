```go run id=owner-decides
package main

import (
	"errors"
	"fmt"
	"slices"
)

type Team struct{ members []string }

// Before: the slice header is copied, but the array behind it is shared.
func (t *Team) MembersLeaky() []string { return t.members }

// After: a clone, so the caller cannot change the team's array.
func (t *Team) Members() []string { return slices.Clone(t.members) }

func (t *Team) AddMember(name string) error {
	if slices.Contains(t.members, name) {
		return nil
	}
	if len(t.members) >= 3 {
		return errors.New("team is full")
	}
	t.members = append(t.members, name)
	return nil
}

func main() {
	t := &Team{}
	for _, n := range []string{"ana", "bo", "cy"} {
		_ = t.AddMember(n)
	}
	t.MembersLeaky()[0] = "zed" // changes the team
	fmt.Println(t.members)
	t.members[0] = "ana"

	t.Members()[0] = "zed" // changes only the copy
	fmt.Println(t.members, t.AddMember("dee"))
	if t.members[0] != "ana" {
		panic("the copy was shared")
	}
	fmt.Println("encapsulate collection: tests pass")
}
```

```output
[zed bo cy]
[ana bo cy] team is full
encapsulate collection: tests pass
```
