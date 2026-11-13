// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

package flatten

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	lists [][]int32
	want  []int32
}{
	{"three", [][]int32{[]int32{1, 2}, []int32{3}, []int32{4, 5, 6}}, []int32{1, 2, 3, 4, 5, 6}},
	{"two", [][]int32{[]int32{7}, []int32{8, 9}}, []int32{7, 8, 9}},
	{"empty", [][]int32{}, []int32{}},
	{"empty-inner", [][]int32{[]int32{}, []int32{1}, []int32{}}, []int32{1}},
	{"all-empty", [][]int32{[]int32{}, []int32{}}, []int32{}},
	{"single", [][]int32{[]int32{5}}, []int32{5}},
	{"one-list", [][]int32{[]int32{1, 2, 3}}, []int32{1, 2, 3}},
	{"negative", [][]int32{[]int32{-1, -2}, []int32{3}}, []int32{-1, -2, 3}},
	{"extremes", [][]int32{[]int32{math.MinInt32}, []int32{2147483647, 0}}, []int32{math.MinInt32, 2147483647, 0}},
}

func TestFlatten(t *testing.T) {
	for _, c := range cases {
		if got := Flatten(c.lists); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestFlattenCopies(t *testing.T) {
	a := make([]int32, 2, 10)
	got := Flatten([][]int32{a, {3}})
	got[0] = 99
	if a[0] != 0 {
		t.Errorf("a change to the result changed the input: %v", a)
	}
}
