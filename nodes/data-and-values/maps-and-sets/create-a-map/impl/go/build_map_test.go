// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

package buildmap

import (
	"slices"
	"testing"
)

var cases = []struct {
	name   string
	pairs  []Pair
	remove []string
	want   []Pair
}{
	{"fruits", []Pair{{"apple", 3}, {"pear", 5}, {"fig", 2}}, []string{}, []Pair{{"apple", 3}, {"fig", 2}, {"pear", 5}}},
	{"update-and-delete", []Pair{{"a", 1}, {"b", 2}, {"a", 7}, {"c", 3}}, []string{"b"}, []Pair{{"a", 7}, {"c", 3}}},
	{"empty", []Pair{}, []string{}, []Pair{}},
	{"replace", []Pair{{"k", 1}, {"k", 2}, {"k", 3}}, []string{}, []Pair{{"k", 3}}},
	{"delete-absent", []Pair{{"a", 1}}, []string{"z"}, []Pair{{"a", 1}}},
	{"delete-all", []Pair{{"a", 1}, {"b", 2}}, []string{"a", "b"}, []Pair{}},
	{"delete-twice", []Pair{{"a", 1}, {"b", 2}}, []string{"a", "a"}, []Pair{{"b", 2}}},
	{"zero-value", []Pair{{"a", 0}, {"b", -4}}, []string{}, []Pair{{"a", 0}, {"b", -4}}},
	{"case", []Pair{{"b", 1}, {"B", 2}, {"a", 3}, {"A", 4}}, []string{}, []Pair{{"A", 4}, {"B", 2}, {"a", 3}, {"b", 1}}},
}

func TestBuildMap(t *testing.T) {
	for _, c := range cases {
		if got := BuildMap(c.pairs, c.remove); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
