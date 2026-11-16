// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

// Package version gives a Version type with a natural order. Go has no
// operator overloading, so the order is a Compare method by convention.
package version

import (
	"cmp"
	"fmt"
	"slices"
	"strconv"
	"strings"
)

// Version is MAJOR.MINOR.PATCH. It is comparable, so == works on it.
type Version struct {
	Major, Minor, Patch int
}

// Parse reads a version such as "1.10.0".
func Parse(s string) (Version, error) {
	p := strings.Split(s, ".")
	if len(p) != 3 {
		return Version{}, fmt.Errorf("version %q: want 3 parts", s)
	}
	var n [3]int
	for i, part := range p {
		v, err := strconv.Atoi(part)
		if err != nil {
			return Version{}, fmt.Errorf("version %q: %w", s, err)
		}
		n[i] = v
	}
	return Version{n[0], n[1], n[2]}, nil
}

// Compare returns -1, 0 or +1. It compares the numbers, most important first.
func (v Version) Compare(o Version) int {
	return cmp.Or(
		cmp.Compare(v.Major, o.Major),
		cmp.Compare(v.Minor, o.Minor),
		cmp.Compare(v.Patch, o.Patch),
	)
}

func (v Version) String() string {
	return fmt.Sprintf("%d.%d.%d", v.Major, v.Minor, v.Patch)
}

// SortVersions sorts the versions by their natural order.
func SortVersions(versions []string) ([]string, error) {
	vs := make([]Version, 0, len(versions))
	for _, s := range versions {
		v, err := Parse(s)
		if err != nil {
			return nil, err
		}
		vs = append(vs, v)
	}
	slices.SortFunc(vs, Version.Compare)
	out := make([]string, len(vs))
	for i, v := range vs {
		out[i] = v.String()
	}
	return out, nil
}
