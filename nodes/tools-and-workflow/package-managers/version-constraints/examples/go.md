```go run id=caret-and-tilde-ranges
// Turn caret, tilde and exact constraints into a range [low, high), then test versions against it.
package main

import (
	"fmt"
	"slices"
	"strconv"
	"strings"
)

type version [3]int

func parse(s string) version {
	var v version
	for i, part := range strings.Split(s, ".") {
		v[i], _ = strconv.Atoi(part)
	}
	return v
}

func (v version) String() string { return fmt.Sprintf("%d.%d.%d", v[0], v[1], v[2]) }

func less(a, b version) bool { return slices.Compare(a[:], b[:]) < 0 }

func toRange(constraint string) (low, high version) {
	op := ""
	if strings.HasPrefix(constraint, "^") || strings.HasPrefix(constraint, "~") {
		op = constraint[:1]
	}
	low = parse(constraint[len(op):])
	major, minor, patch := low[0], low[1], low[2]
	switch {
	case op == "^" && major > 0: // keep the left-most part that is not zero
		high = version{major + 1, 0, 0}
	case op == "^" && minor > 0:
		high = version{0, minor + 1, 0}
	case op == "^":
		high = version{0, 0, patch + 1}
	case op == "~": // allow patch changes only
		high = version{major, minor + 1, 0}
	default: // an exact version
		high = version{major, minor, patch + 1}
	}
	return low, high
}

func main() {
	candidates := []string{"0.0.4", "0.0.5", "0.2.3", "0.2.4", "0.2.9", "0.3.0",
		"1.2.2", "1.2.3", "1.2.9", "1.4.0", "2.0.0"}
	fmt.Println("versions:", strings.Join(candidates, " "))
	for _, constraint := range []string{"^1.2.3", "^0.2.4", "^0.0.4", "~1.2.3", "~0.2.4", "1.2.3"} {
		low, high := toRange(constraint)
		var matches []string
		for _, c := range candidates {
			if v := parse(c); !less(v, low) && less(v, high) {
				matches = append(matches, c)
			}
		}
		fmt.Printf("%s means >=%v <%v, matches %s\n", constraint, low, high, strings.Join(matches, " "))
	}
}
```

```output
versions: 0.0.4 0.0.5 0.2.3 0.2.4 0.2.9 0.3.0 1.2.2 1.2.3 1.2.9 1.4.0 2.0.0
^1.2.3 means >=1.2.3 <2.0.0, matches 1.2.3 1.2.9 1.4.0
^0.2.4 means >=0.2.4 <0.3.0, matches 0.2.4 0.2.9
^0.0.4 means >=0.0.4 <0.0.5, matches 0.0.4
~1.2.3 means >=1.2.3 <1.3.0, matches 1.2.3 1.2.9
~0.2.4 means >=0.2.4 <0.3.0, matches 0.2.4 0.2.9
1.2.3 means >=1.2.3 <1.2.4, matches 1.2.3
```

- Verdict: idiom. A version is an array of three ints, and `slices.Compare` compares two arrays element by element.
- The method `String` lets `%v` print a version as 1.2.3.
- For module versions, the package golang.org/x/mod/semver compares versions that start with a v.
