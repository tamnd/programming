// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

package total

import (
	"math"
	"testing"
)

var cases = []struct {
	name  string
	items []int32
	want  int64
}{
	{"three", []int32{1, 2, 3}, 6},
	{"mixed", []int32{10, -4, 7, 0, 3}, 16},
	{"empty", []int32{}, 0},
	{"single", []int32{7}, 7},
	{"negative", []int32{-5, 2, -1}, -4},
	{"limit", []int32{math.MaxInt32, math.MaxInt32, 2}, 4294967296},
	{"limit-low", []int32{math.MinInt32, math.MinInt32}, -4294967296},
}

func TestTotal(t *testing.T) {
	for _, c := range cases {
		if got := Total(c.items); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}
