// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

package cartesian

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	lists [][]int32
	want  [][]int32
}{
	{"two", [][]int32{{1, 2}, {3, 4}}, [][]int32{{1, 3}, {1, 4}, {2, 3}, {2, 4}}},
	{"three", [][]int32{{1, 2}, {3}, {4, 5}}, [][]int32{{1, 3, 4}, {1, 3, 5}, {2, 3, 4}, {2, 3, 5}}},
	{"no-lists", [][]int32{}, [][]int32{{}}},
	{"one-empty", [][]int32{{1, 2}, {}}, [][]int32{}},
	{"single", [][]int32{{7, 8, 9}}, [][]int32{{7}, {8}, {9}}},
	{"duplicates", [][]int32{{1, 1}, {2}}, [][]int32{{1, 2}, {1, 2}}},
	{"negative", [][]int32{{-1}, {-2, -3}}, [][]int32{{-1, -2}, {-1, -3}}},
	{"extremes", [][]int32{{math.MinInt32, math.MaxInt32}, {0}}, [][]int32{{math.MinInt32, 0}, {math.MaxInt32, 0}}},
}

func TestProduct(t *testing.T) {
	for _, c := range cases {
		got := Product(c.lists)
		if !slices.EqualFunc(got, c.want, slices.Equal[[]int32]) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestProductOwnArrays(t *testing.T) {
	got := Product([][]int32{{1, 2}, {3, 4}})
	got[0][0] = 99
	if got[1][0] != 1 {
		t.Errorf("a change to one tuple changed another: %v", got)
	}
}
