// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

package minmax

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name    string
	numbers []int32
	lo, hi  int32
}{
	{"mixed", []int32{3, 1, 2}, 1, 3},
	{"longer", []int32{4, 9, -2, 7, 0}, -2, 9},
	{"one", []int32{5}, 5, 5},
	{"negative", []int32{-3, -1, -7}, -7, -1},
	{"all-same", []int32{2, 2, 2}, 2, 2},
	{"int32-extremes", []int32{math.MaxInt32, 0, math.MinInt32}, math.MinInt32, math.MaxInt32},
	{"sorted", []int32{1, 2, 3, 4}, 1, 4},
	{"reversed", []int32{4, 3, 2, 1}, 1, 4},
}

func TestMinMax(t *testing.T) {
	for _, c := range cases {
		before := slices.Clone(c.numbers)
		lo, hi := MinMax(c.numbers)
		if lo != c.lo || hi != c.hi {
			t.Errorf("%s: got (%d, %d), want (%d, %d)", c.name, lo, hi, c.lo, c.hi)
		}
		if !slices.Equal(c.numbers, before) {
			t.Errorf("%s: the input changed", c.name)
		}
	}
}
