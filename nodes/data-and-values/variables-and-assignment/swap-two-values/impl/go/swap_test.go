// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

package swap

import (
	"slices"
	"testing"
)

var cases = []struct {
	name string
	xs   []int64
	i, j int
	want []int64
}{
	{"first-and-last", []int64{1, 2, 3}, 0, 2, []int64{3, 2, 1}},
	{"middle-pair", []int64{10, 20, 30, 40}, 1, 2, []int64{10, 30, 20, 40}},
	{"two-items", []int64{5, 7}, 0, 1, []int64{7, 5}},
	{"single-item", []int64{9}, 0, 0, []int64{9}},
	{"same-index", []int64{1, 2, 3}, 1, 1, []int64{1, 2, 3}},
	{"higher-index-first", []int64{1, 2, 3, 4}, 3, 0, []int64{4, 2, 3, 1}},
	{"equal-values", []int64{4, 4}, 0, 1, []int64{4, 4}},
	{"negative-values", []int64{-1, 0, 1}, 0, 2, []int64{1, 0, -1}},
	{"int32-extremes", []int64{2147483647, -2147483648}, 0, 1, []int64{-2147483648, 2147483647}},
	{"eight-items", []int64{1, 2, 3, 4, 5, 6, 7, 8}, 2, 6, []int64{1, 2, 7, 4, 5, 6, 3, 8}},
}

func TestSwapItems(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			before := slices.Clone(c.xs)
			if got := SwapItems(c.xs, c.i, c.j); !slices.Equal(got, c.want) {
				t.Fatalf("SwapItems(%v, %d, %d) = %v, want %v", c.xs, c.i, c.j, got, c.want)
			}
			if !slices.Equal(c.xs, before) {
				t.Fatalf("SwapItems changed its input to %v", c.xs)
			}
		})
	}
}
