// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

package permutations

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int32
	want  [][]int32
}{
	{"three", []int32{1, 2, 3}, [][]int32{{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}}},
	{"two", []int32{5, 6}, [][]int32{{5, 6}, {6, 5}}},
	{"empty", []int32{}, [][]int32{{}}},
	{"single", []int32{7}, [][]int32{{7}}},
	{"duplicates", []int32{1, 1, 2}, [][]int32{{1, 1, 2}, {1, 2, 1}, {1, 1, 2}, {1, 2, 1}, {2, 1, 1}, {2, 1, 1}}},
	{"unsorted", []int32{3, 1, 2}, [][]int32{{3, 1, 2}, {3, 2, 1}, {1, 3, 2}, {1, 2, 3}, {2, 3, 1}, {2, 1, 3}}},
	{"extremes", []int32{math.MinInt32, math.MaxInt32}, [][]int32{{math.MinInt32, math.MaxInt32}, {math.MaxInt32, math.MinInt32}}},
}

func TestPermutations(t *testing.T) {
	for _, c := range cases {
		got := Permutations(c.items)
		if !slices.EqualFunc(got, c.want, slices.Equal[[]int32]) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestPermutationsOwnArrays(t *testing.T) {
	got := Permutations([]int32{1, 2})
	got[0][0] = 99
	if got[1][1] != 1 {
		t.Errorf("a change to one order changed another: %v", got)
	}
}
