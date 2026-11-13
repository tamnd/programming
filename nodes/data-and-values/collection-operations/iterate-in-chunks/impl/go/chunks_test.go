// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

package chunks

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	size  int
	want  [][]int
}{
	{"pairs", []int{1, 2, 3, 4, 5}, 2, [][]int{{1, 2}, {3, 4}, {5}}},
	{"triples", []int{1, 2, 3, 4, 5, 6}, 3, [][]int{{1, 2, 3}, {4, 5, 6}}},
	{"empty", []int{}, 3, [][]int{}},
	{"size-one", []int{7, 8, 9}, 1, [][]int{{7}, {8}, {9}}},
	{"size-larger", []int{1, 2}, 5, [][]int{{1, 2}}},
	{"size-equal", []int{1, 2, 3}, 3, [][]int{{1, 2, 3}}},
	{"last-shorter", []int{1, 2, 3, 4}, 3, [][]int{{1, 2, 3}, {4}}},
	{"negative", []int{-1, -2, -3}, 2, [][]int{{-1, -2}, {-3}}},
	{"extremes", []int{-2147483648, 2147483647, 0}, 2, [][]int{{-2147483648, 2147483647}, {0}}},
}

func TestChunks(t *testing.T) {
	for _, c := range cases {
		got := Chunks(c.items, c.size)
		if !slices.EqualFunc(got, c.want, slices.Equal[[]int]) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestAppendDoesNotChangeItems(t *testing.T) {
	items := []int{1, 2, 3, 4}
	c := Chunks(items, 2)
	_ = append(c[0], 99)
	if !slices.Equal(items, []int{1, 2, 3, 4}) {
		t.Errorf("the input changed: %v", items)
	}
}
