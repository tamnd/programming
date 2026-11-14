// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

package lookup

import (
	"maps"
	"slices"
	"testing"
)

var cases = []struct {
	name     string
	table    map[string]int
	keys     []string
	fallback int
	want     []int
}{
	{"prices", map[string]int{"apple": 3, "pear": 5}, []string{"apple", "fig", "pear"}, 0, []int{3, 0, 5}},
	{"negative-fallback", map[string]int{"a": 1}, []string{"b", "a"}, -1, []int{-1, 1}},
	{"empty-table", map[string]int{}, []string{"a", "b"}, 7, []int{7, 7}},
	{"empty-keys", map[string]int{"a": 1}, []string{}, 0, []int{}},
	{"zero-value", map[string]int{"a": 0}, []string{"a", "b"}, 5, []int{0, 5}},
	{"repeat", map[string]int{"a": 1}, []string{"b", "b", "a"}, 2, []int{2, 2, 1}},
	{"case", map[string]int{"a": 1}, []string{"A"}, 0, []int{0}},
}

func TestLookup(t *testing.T) {
	for _, c := range cases {
		before := maps.Clone(c.table)
		if got := Lookup(c.table, c.keys, c.fallback); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if !maps.Equal(before, c.table) {
			t.Errorf("%s: the table changed", c.name)
		}
	}
}

func TestNilTable(t *testing.T) {
	// A read from a nil map is fine and gives no entry.
	if got := Lookup(nil, []string{"a"}, 4); !slices.Equal(got, []int{4}) {
		t.Errorf("nil table: got %v", got)
	}
}
