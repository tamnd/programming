```go run id=precedence
package main

import (
	"cmp"
	"fmt"
	"regexp"
	"slices"
	"strconv"
	"strings"
)

var semver = regexp.MustCompile(`^(?P<major>0|[1-9]\d*)\.(?P<minor>0|[1-9]\d*)\.(?P<patch>0|[1-9]\d*)` +
	`(?:-(?P<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?` +
	`(?:\+(?P<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$`)

type Version struct {
	Major, Minor, Patch string
	Pre                 []string
	Build               string
}

func Parse(text string) (Version, error) {
	m := semver.FindStringSubmatch(text)
	if m == nil {
		return Version{}, fmt.Errorf("not a semantic version: %q", text)
	}
	v := Version{Major: m[1], Minor: m[2], Patch: m[3], Build: m[5]}
	if m[4] != "" {
		v.Pre = strings.Split(m[4], ".")
	}
	return v, nil
}

func isNumeric(s string) bool {
	return strings.Trim(s, "0123456789") == ""
}

// compareNumeric compares two decimal strings with no leading zeros.
// A longer string is a larger number, so no integer type can overflow.
func compareNumeric(a, b string) int {
	return cmp.Or(cmp.Compare(len(a), len(b)), strings.Compare(a, b))
}

func compareIdentifier(a, b string) int {
	switch an, bn := isNumeric(a), isNumeric(b); {
	case an && bn:
		return compareNumeric(a, b)
	case an:
		return -1
	case bn:
		return 1
	default:
		return strings.Compare(a, b)
	}
}

// Compare returns the precedence of a and b. It ignores build metadata.
func Compare(a, b Version) int {
	if c := cmp.Or(compareNumeric(a.Major, b.Major), compareNumeric(a.Minor, b.Minor), compareNumeric(a.Patch, b.Patch)); c != 0 {
		return c
	}
	switch {
	case len(a.Pre) == 0 && len(b.Pre) == 0:
		return 0
	case len(a.Pre) == 0:
		return 1
	case len(b.Pre) == 0:
		return -1
	}
	for i := range min(len(a.Pre), len(b.Pre)) {
		if c := compareIdentifier(a.Pre[i], b.Pre[i]); c != 0 {
			return c
		}
	}
	return cmp.Compare(len(a.Pre), len(b.Pre))
}

func main() {
	texts := []string{"1.0.0", "1.0.0-rc.1", "1.0.0-beta.11", "1.0.0-beta.2", "1.0.0-beta",
		"1.0.0-alpha.beta", "1.0.0-alpha.1", "1.0.0-alpha", "1.0.0-alpha.99999999999999999999"}
	var versions []Version
	for _, text := range texts {
		v, err := Parse(text)
		if err != nil {
			fmt.Println(err)
			return
		}
		versions = append(versions, v)
	}
	slices.SortFunc(versions, Compare)
	for _, v := range versions {
		fmt.Println(v.Major+"."+v.Minor+"."+v.Patch, v.Pre)
	}

	a, _ := Parse("1.4.2+build.7")
	b, _ := Parse("1.4.2+build.8")
	fmt.Println("precedence:", Compare(a, b), "build:", a.Build, b.Build)

	_, err := strconv.Atoi("99999999999999999999")
	fmt.Println("Atoi:", err)

	for _, text := range []string{"v1.2.3", "1.2", "01.2.3", "1.0.0-01"} {
		_, err := Parse(text)
		fmt.Println(err)
	}
}
```

```output for=precedence
1.0.0 [alpha]
1.0.0 [alpha 1]
1.0.0 [alpha 99999999999999999999]
1.0.0 [alpha beta]
1.0.0 [beta]
1.0.0 [beta 2]
1.0.0 [beta 11]
1.0.0 [rc 1]
1.0.0 []
precedence: 0 build: build.7 build.8
Atoi: strconv.Atoi: parsing "99999999999999999999": value out of range
not a semantic version: "v1.2.3"
not a semantic version: "1.2"
not a semantic version: "01.2.3"
not a semantic version: "1.0.0-01"
```

- Verdict: idiom. The Go standard library has no SemVer package. The package `regexp` accepts the official regex with named groups, and `slices.SortFunc` sorts with a comparison function.
- The example keeps each number as a string and compares the length first. The specification sets no limit on the size of a number, and `strconv.Atoi` fails on a value larger than `int`.
- The module `golang.org/x/mod/semver` has `Compare`, `IsValid`, `Canonical`, `Major` and `Sort`. Its documentation says that it follows SemVer 2.0.0 with two exceptions: a version must start with "v", and the shorthands vMAJOR and vMAJOR.MINOR are accepted. It is not in the standard library.
- Go module versions start with "v". The go command ignores build metadata when it compares versions.
