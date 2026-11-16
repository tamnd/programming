// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

package serveorder

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name       string
	priorities []int32
	want       []int
}{
	{"example", []int32{2, 5, 1, 5}, []int{1, 3, 0, 2}},
	{"longer", []int32{3, 1, 4, 1, 5, 9, 2, 6}, []int{5, 7, 4, 2, 0, 6, 1, 3}},
	{"empty", []int32{}, []int{}},
	{"one", []int32{7}, []int{0}},
	{"ties", []int32{4, 4, 4, 4}, []int{0, 1, 2, 3}},
	{"negative", []int32{-3, -1, -2}, []int{1, 2, 0}},
	{"int32-extremes", []int32{math.MinInt32, math.MaxInt32, 0, math.MinInt32}, []int{1, 2, 0, 3}},
	{"sorted", []int32{1, 2, 3}, []int{2, 1, 0}},
	{"reversed", []int32{3, 2, 1}, []int{0, 1, 2}},
}

func TestServeOrder(t *testing.T) {
	for _, c := range cases {
		before := slices.Clone(c.priorities)
		if got := ServeOrder(c.priorities); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if !slices.Equal(c.priorities, before) {
			t.Errorf("%s: the input changed", c.name)
		}
	}
}
