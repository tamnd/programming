// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

package limits

import (
	"math"
	"testing"
)

var cases = []struct {
	name                   string
	value, low, high, want int32
}{
	{"inside", 5, 1, 10, 5},
	{"below", -3, 0, 10, 0},
	{"above", 42, 0, 10, 10},
	{"at-low", 0, 0, 10, 0},
	{"at-high", 10, 0, 10, 10},
	{"equal-bounds", 7, 3, 3, 3},
	{"negative-range", -50, -20, -10, -20},
	{"zero", 0, -5, 5, 0},
	{"int-max", math.MaxInt32, -100, 100, 100},
	{"int-min", math.MinInt32, -100, 100, -100},
	{"full-range", math.MinInt32, math.MinInt32, math.MaxInt32, math.MinInt32},
}

func TestClamp(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := Clamp(c.value, c.low, c.high); got != c.want {
				t.Fatalf("got %d, want %d", got, c.want)
			}
		})
	}
}
