// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

package meanmedian

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name         string
	items        []int64
	mean, median float64
}{
	{"odd", []int64{3, 1, 2}, 2.0, 2.0},
	{"even", []int64{4, 1, 3, 2}, 2.5, 2.5},
	{"skewed", []int64{1, 2, 3, 4, 100}, 22.0, 3.0},
	{"single", []int64{5}, 5.0, 5.0},
	{"two-items", []int64{1, 2}, 1.5, 1.5},
	{"negative", []int64{-3, -1, -2}, -2.0, -2.0},
	{"unsorted-even", []int64{10, 2, 8, 4}, 6.0, 6.0},
	{"duplicates", []int64{1, 1, 1, 5}, 2.0, 1.0},
	{"not-an-integer", []int64{1, 2, 2}, 1.6666666666666667, 2.0},
	{"large", []int64{9223372036854775807, 9223372036854775807}, 9223372036854775808.0, 9223372036854775808.0},
}

func near(got, want float64) bool {
	return math.Abs(got-want) <= math.Max(1e-9, 1e-12*math.Abs(want))
}

func TestMeanAndMedian(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			before := slices.Clone(c.items)
			mean, median := MeanAndMedian(c.items)
			if !near(mean, c.mean) || !near(median, c.median) {
				t.Fatalf("got (%v, %v), want (%v, %v)", mean, median, c.mean, c.median)
			}
			if !slices.Equal(before, c.items) {
				t.Fatal("the slice changed")
			}
		})
	}
}
