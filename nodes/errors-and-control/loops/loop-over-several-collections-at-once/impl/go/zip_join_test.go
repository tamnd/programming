// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

package zipjoin

import (
	"slices"
	"testing"
)

var cases = []struct {
	name         string
	lower, upper []string
	digits       []int32
	want         []string
}{
	{"rosetta", []string{"a", "b", "c"}, []string{"A", "B", "C"}, []int32{1, 2, 3}, []string{"aA1", "bB2", "cC3"}},
	{"words", []string{"x", "y"}, []string{"X", "Y"}, []int32{10, 20}, []string{"xX10", "yY20"}},
	{"shorter-first", []string{"a"}, []string{"A", "B"}, []int32{1, 2, 3}, []string{"aA1"}},
	{"shorter-last", []string{"a", "b", "c"}, []string{"A", "B", "C"}, []int32{1}, []string{"aA1"}},
	{"one-empty", []string{"a", "b"}, []string{}, []int32{1, 2}, []string{}},
	{"all-empty", []string{}, []string{}, []int32{}, []string{}},
	{"negative", []string{"a"}, []string{"A"}, []int32{-5}, []string{"aA-5"}},
	{"empty-strings", []string{"", "b"}, []string{"A", ""}, []int32{1, 2}, []string{"A1", "b2"}},
}

func TestZipJoin(t *testing.T) {
	for _, c := range cases {
		if got := ZipJoin(c.lower, c.upper, c.digits); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
