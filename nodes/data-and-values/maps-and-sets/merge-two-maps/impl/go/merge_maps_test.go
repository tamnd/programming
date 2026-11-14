// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

package mergemaps

import (
	"maps"
	"slices"
	"testing"
)

var cases = []struct {
	name     string
	base     map[string]int
	override map[string]int
	want     []Pair
}{
	{"prices", map[string]int{"apple": 3, "pear": 5}, map[string]int{"pear": 6, "fig": 2}, []Pair{{"apple", 3}, {"fig", 2}, {"pear", 6}}},
	{"disjoint", map[string]int{"a": 1}, map[string]int{"b": 2}, []Pair{{"a", 1}, {"b", 2}}},
	{"empty-base", map[string]int{}, map[string]int{"a": 1}, []Pair{{"a", 1}}},
	{"empty-override", map[string]int{"a": 1}, map[string]int{}, []Pair{{"a", 1}}},
	{"both-empty", map[string]int{}, map[string]int{}, []Pair{}},
	{"same-keys", map[string]int{"a": 1, "b": 2}, map[string]int{"a": 9, "b": 8}, []Pair{{"a", 9}, {"b", 8}}},
	{"zero-wins", map[string]int{"a": 5}, map[string]int{"a": 0}, []Pair{{"a", 0}}},
	{"case", map[string]int{"a": 1}, map[string]int{"A": 2}, []Pair{{"A", 2}, {"a", 1}}},
}

func TestMergeMaps(t *testing.T) {
	for _, c := range cases {
		base, override := maps.Clone(c.base), maps.Clone(c.override)
		if got := MergeMaps(c.base, c.override); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if !maps.Equal(base, c.base) || !maps.Equal(override, c.override) {
			t.Errorf("%s: an input changed", c.name)
		}
	}
}

func TestNilBase(t *testing.T) {
	if got := MergeMaps(nil, map[string]int{"a": 1}); !slices.Equal(got, []Pair{{"a", 1}}) {
		t.Errorf("nil base: got %v", got)
	}
}
