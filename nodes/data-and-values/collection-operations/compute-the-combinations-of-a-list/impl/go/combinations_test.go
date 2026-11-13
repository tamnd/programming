// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

package combinations

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int32
	k     int
	want  [][]int32
}{
	{"two-of-four", []int32{1, 2, 3, 4}, 2, [][]int32{{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}},
	{"three-of-four", []int32{1, 2, 3, 4}, 3, [][]int32{{1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4}}},
	{"zero-k", []int32{1, 2}, 0, [][]int32{{}}},
	{"empty", []int32{}, 0, [][]int32{{}}},
	{"k-too-large", []int32{1, 2}, 3, [][]int32{}},
	{"all", []int32{1, 2, 3}, 3, [][]int32{{1, 2, 3}}},
	{"duplicates", []int32{1, 1, 2}, 2, [][]int32{{1, 1}, {1, 2}, {1, 2}}},
	{"unsorted", []int32{3, 1, 2}, 2, [][]int32{{3, 1}, {3, 2}, {1, 2}}},
	{"extremes", []int32{math.MinInt32, math.MaxInt32}, 1, [][]int32{{math.MinInt32}, {math.MaxInt32}}},
}

func TestCombinations(t *testing.T) {
	for _, c := range cases {
		got := Combinations(c.items, c.k)
		if !slices.EqualFunc(got, c.want, slices.Equal[[]int32]) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestCombinationsOwnArrays(t *testing.T) {
	got := Combinations([]int32{1, 2, 3}, 2)
	got[0][0] = 99
	if got[1][0] != 1 {
		t.Errorf("a change to one choice changed another: %v", got)
	}
}
