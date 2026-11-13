// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

package rotateleft

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	k     int
	want  []int
}{
	{"by-two", []int{1, 2, 3, 4, 5}, 2, []int{3, 4, 5, 1, 2}},
	{"by-one", []int{1, 2, 3}, 1, []int{2, 3, 1}},
	{"zero", []int{1, 2, 3}, 0, []int{1, 2, 3}},
	{"full-turn", []int{1, 2, 3}, 3, []int{1, 2, 3}},
	{"more-than-length", []int{1, 2, 3}, 4, []int{2, 3, 1}},
	{"much-larger", []int{1, 2, 3}, 2000000000, []int{3, 1, 2}},
	{"empty", []int{}, 5, []int{}},
	{"empty-zero", []int{}, 0, []int{}},
	{"single", []int{7}, 3, []int{7}},
	{"duplicates", []int{1, 1, 2}, 1, []int{1, 2, 1}},
	{"negative-items", []int{-1, -2, -3}, 1, []int{-2, -3, -1}},
}

func TestRotateLeft(t *testing.T) {
	for _, c := range cases {
		if got := RotateLeft(c.items, c.k); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestInputDoesNotChange(t *testing.T) {
	items := []int{1, 2, 3}
	RotateLeft(items, 1)
	if !slices.Equal(items, []int{1, 2, 3}) {
		t.Errorf("the input changed: %v", items)
	}
}
