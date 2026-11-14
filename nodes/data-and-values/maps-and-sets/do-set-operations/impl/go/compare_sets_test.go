// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

package compare

import (
	"slices"
	"testing"
)

var cases = []struct {
	name string
	a, b []string
	want Result
}{
	{"colors", []string{"red", "blue", "green"}, []string{"blue", "yellow"}, Result{[]string{"blue", "green", "red", "yellow"}, []string{"blue"}, []string{"green", "red"}, []string{"green", "red", "yellow"}, false}},
	{"subset", []string{"b"}, []string{"a", "b", "c"}, Result{[]string{"a", "b", "c"}, []string{"b"}, []string{}, []string{"a", "c"}, true}},
	{"both-empty", []string{}, []string{}, Result{[]string{}, []string{}, []string{}, []string{}, true}},
	{"empty-a", []string{}, []string{"x"}, Result{[]string{"x"}, []string{}, []string{}, []string{"x"}, true}},
	{"empty-b", []string{"x"}, []string{}, Result{[]string{"x"}, []string{}, []string{"x"}, []string{"x"}, false}},
	{"equal", []string{"a", "b"}, []string{"b", "a"}, Result{[]string{"a", "b"}, []string{"a", "b"}, []string{}, []string{}, true}},
	{"disjoint", []string{"a"}, []string{"b"}, Result{[]string{"a", "b"}, []string{}, []string{"a"}, []string{"a", "b"}, false}},
	{"duplicates", []string{"a", "a", "b"}, []string{"b", "b"}, Result{[]string{"a", "b"}, []string{"b"}, []string{"a"}, []string{"a"}, false}},
	{"case", []string{"a"}, []string{"A"}, Result{[]string{"A", "a"}, []string{}, []string{"a"}, []string{"A", "a"}, false}},
}

func TestCompareSets(t *testing.T) {
	for _, c := range cases {
		got := CompareSets(c.a, c.b)
		// slices.Equal treats a nil slice and an empty slice as equal.
		ok := slices.Equal(got.Union, c.want.Union) &&
			slices.Equal(got.Intersection, c.want.Intersection) &&
			slices.Equal(got.Difference, c.want.Difference) &&
			slices.Equal(got.SymmetricDifference, c.want.SymmetricDifference) &&
			got.Subset == c.want.Subset
		if !ok {
			t.Errorf("%s: got %+v, want %+v", c.name, got, c.want)
		}
	}
}
