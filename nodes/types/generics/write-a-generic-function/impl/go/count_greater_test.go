// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

package generic

import (
	"math"
	"testing"
)

var cases = []struct {
	name  string
	items []int32
	limit int32
	want  int
}{
	{"some", []int32{34, 50, 25, 100, 65}, 50, 2},
	{"all", []int32{3, 4, 5}, 2, 3},
	{"none", []int32{3, 4, 5}, 9, 0},
	{"empty", nil, 0, 0},
	{"equal-not-counted", []int32{5, 5, 5}, 5, 0},
	{"duplicates", []int32{7, 1, 7, 7}, 6, 3},
	{"negative", []int32{-3, -1, 0, 2}, -2, 3},
	{"min-limit", []int32{math.MinInt32, 0, math.MaxInt32}, math.MinInt32, 2},
	{"max-limit", []int32{math.MaxInt32, 1}, math.MaxInt32, 0},
	{"single", []int32{1}, 0, 1},
}

func TestCountGreater(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := CountGreater(c.items, c.limit); got != c.want {
				t.Fatalf("got %d, want %d", got, c.want)
			}
		})
	}
}

func TestStrings(t *testing.T) {
	if got := CountGreater([]string{"pear", "apple", "plum"}, "orange"); got != 2 {
		t.Fatalf("got %d, want 2", got)
	}
}
