```go run id=backtracking-resolver
// A backtracking dependency resolver: try the newest version first, and go back on a conflict.
package main

import (
	"fmt"
	"maps"
	"slices"
	"strconv"
	"strings"
)

// req says that who needs name in the range [lo, hi).
type req struct{ name, lo, hi, who string }

var versions = map[string]map[string][]req{
	"web": {
		"2.0": {{"log", "2.0", "3.0", ""}},
		"1.1": {{"log", "1.1", "2.0", ""}},
		"1.0": {{"log", "1.0", "2.0", ""}},
	},
	"log": {"2.0": nil, "1.2": nil, "1.1": nil, "1.0": nil},
}

func cmp(a, b string) int {
	pa, pb := strings.Split(a, "."), strings.Split(b, ".")
	for i := range pa {
		x, _ := strconv.Atoi(pa[i])
		y, _ := strconv.Atoi(pb[i])
		if x != y {
			return x - y
		}
	}
	return 0
}

func fits(v, lo, hi string) bool { return cmp(lo, v) <= 0 && cmp(v, hi) < 0 }

func solve(todo []req, chosen map[string]string) map[string]string {
	if len(todo) == 0 {
		return chosen
	}
	r, rest := todo[0], todo[1:]
	if v, ok := chosen[r.name]; ok {
		if fits(v, r.lo, r.hi) {
			return solve(rest, chosen)
		}
		fmt.Printf("  conflict: %s needs %s >=%s,<%s, but %s %s is chosen\n", r.who, r.name, r.lo, r.hi, r.name, v)
		return nil
	}
	list := slices.SortedFunc(maps.Keys(versions[r.name]), func(a, b string) int { return cmp(b, a) })
	for _, v := range list {
		if !fits(v, r.lo, r.hi) {
			continue
		}
		fmt.Println("try", r.name, v)
		next := slices.Clone(rest)
		for _, d := range versions[r.name][v] {
			next = append(next, req{d.name, d.lo, d.hi, r.name + " " + v})
		}
		c := maps.Clone(chosen)
		c[r.name] = v
		if result := solve(next, c); result != nil {
			return result
		}
	}
	return nil
}

func main() {
	roots := [][]req{
		{{"web", "1.0", "3.0", "app"}, {"log", "1.0", "2.0", "app"}},
		{{"web", "2.0", "3.0", "app"}, {"log", "1.0", "2.0", "app"}},
	}
	for _, root := range roots {
		var needs []string
		for _, r := range root {
			needs = append(needs, fmt.Sprintf("%s >=%s,<%s", r.name, r.lo, r.hi))
		}
		fmt.Println("app needs", strings.Join(needs, ", "))
		result := solve(root, map[string]string{})
		if result == nil {
			fmt.Println("result: no solution")
			continue
		}
		var parts []string
		for _, n := range slices.Sorted(maps.Keys(result)) {
			parts = append(parts, n+" "+result[n])
		}
		fmt.Println("result:", strings.Join(parts, ", "))
	}
}
```

```output
app needs web >=1.0,<3.0, log >=1.0,<2.0
try web 2.0
try log 1.2
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.2 is chosen
try log 1.1
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.1 is chosen
try log 1.0
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.0 is chosen
try web 1.1
try log 1.2
result: log 1.2, web 1.1
app needs web >=2.0,<3.0, log >=1.0,<2.0
try web 2.0
try log 1.2
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.2 is chosen
try log 1.1
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.1 is chosen
try log 1.0
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.0 is chosen
result: no solution
```

- Verdict: idiom. A map of maps holds the versions, and a struct holds one requirement.
- `maps.Clone` copies the chosen versions for each try, and `slices.Clone` copies the open requirements, so that `append` does not change the slice of the caller.
- Go map order is random, so `slices.SortedFunc` sorts the versions, newest first, and `slices.Sorted` sorts the names of the result.
