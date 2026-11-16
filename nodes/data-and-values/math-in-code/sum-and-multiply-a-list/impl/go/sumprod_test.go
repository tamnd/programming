// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

package sumprod

import (
	"math"
	"testing"
)

var cases = []struct {
	name         string
	items        []int64
	sum, product int64
}{
	{"small", []int64{1, 2, 3, 4}, 10, 24},
	{"mixed", []int64{3, -2, 5}, 6, -30},
	{"empty", []int64{}, 0, 1},
	{"single", []int64{7}, 7, 7},
	{"with-zero", []int64{5, 0, 9}, 14, 0},
	{"two-negatives", []int64{-2, -3}, -5, 6},
	{"three-negatives", []int64{-1, -2, -3}, -6, -6},
	{"ones", []int64{1, 1, 1}, 3, 1},
	{"large-product", []int64{3037000499, 3037000499}, 6074000998, 9223372030926249001},
	{"to-the-limit", []int64{9223372036854775806, 1}, 9223372036854775807, 9223372036854775806},
	{"beyond-double", []int64{9007199254740993}, 9007199254740993, 9007199254740993},
	{"min", []int64{math.MinInt64}, math.MinInt64, math.MinInt64},
}

func TestSumAndProduct(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			sum, product := SumAndProduct(c.items)
			if sum != c.sum || product != c.product {
				t.Fatalf("got (%d, %d), want (%d, %d)", sum, product, c.sum, c.product)
			}
		})
	}
}

func TestWrapAround(t *testing.T) {
	if _, product := SumAndProduct([]int64{1 << 32, 1 << 32}); product != 0 {
		t.Fatalf("got %d, want 0", product)
	}
	if sum, _ := SumAndProduct([]int64{math.MaxInt64, 1}); sum != math.MinInt64 {
		t.Fatalf("got %d", sum)
	}
}
