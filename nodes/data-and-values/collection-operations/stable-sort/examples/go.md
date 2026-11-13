```go run id=sort-stable-func
package main

import (
	"cmp"
	"fmt"
	"slices"
	"sort"
)

type staff struct {
	name  string
	team  string
	grade int
}

func names(s []staff) []string {
	out := make([]string, len(s))
	for i, p := range s {
		out[i] = p.name
	}
	return out
}

func main() {
	all := []staff{{"ann", "ops", 3}, {"bob", "dev", 2}, {"cy", "ops", 2}, {"di", "dev", 3}, {"ed", "ops", 2}}

	// slices.SortStableFunc keeps equal teams in the name order.
	s := slices.Clone(all)
	slices.SortStableFunc(s, func(a, b staff) int { return cmp.Compare(a.team, b.team) })
	fmt.Println(names(s))

	// sort.SliceStable is the older form with a less function.
	s = slices.Clone(all)
	sort.SliceStable(s, func(i, j int) bool { return s[i].grade < s[j].grade })
	sort.SliceStable(s, func(i, j int) bool { return s[i].team < s[j].team })
	fmt.Println(names(s))

	// slices.SortFunc is not stable. A full key, here team, grade and name,
	// leaves no equal items, so the result is the same on every run.
	s = slices.Clone(all)
	slices.SortFunc(s, func(a, b staff) int {
		return cmp.Or(cmp.Compare(a.team, b.team), cmp.Compare(a.grade, b.grade), cmp.Compare(a.name, b.name))
	})
	fmt.Println(names(s))
}
```

```output
[bob di ann cy ed]
[bob di cy ed ann]
[bob di cy ed ann]
```

- Verdict: native. `slices.SortStableFunc` keeps the name order inside each team. Two `sort.SliceStable` passes sort by team and then by grade. `slices.SortFunc` is not stable, so the program compares the team, the grade and the name, and no two items are equal.
