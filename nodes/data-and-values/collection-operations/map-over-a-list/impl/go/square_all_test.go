// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

package squareall

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	want  []int
}{
	{"small", []int{1, 2, 3}, []int{1, 4, 9}},
	{"tens", []int{5, 10}, []int{25, 100}},
	{"empty", []int{}, []int{}},
	{"single", []int{7}, []int{49}},
	{"negative", []int{-3, -1}, []int{9, 1}},
	{"zero", []int{0}, []int{0}},
	{"limit", []int{46340, -46340}, []int{2147395600, 2147395600}},
	{"order", []int{3, 1, 2}, []int{9, 1, 4}},
	{"duplicates", []int{2, 2}, []int{4, 4}},
}

func TestSquareAll(t *testing.T) {
	for _, c := range cases {
		if got := SquareAll(c.items); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
