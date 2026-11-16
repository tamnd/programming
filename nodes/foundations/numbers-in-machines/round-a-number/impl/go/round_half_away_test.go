// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.

package rounding

import (
	"math"
	"testing"
)

var cases = []struct {
	name string
	x    float64
	want int64
}{
	{"down", 2.4, 2},
	{"up", 2.6, 3},
	{"negative", -2.6, -3},
	{"integer", 7.0, 7},
	{"half-even-up", 2.5, 3},
	{"half-odd-up", 1.5, 2},
	{"half-zero", 0.5, 1},
	{"half-negative", -2.5, -3},
	{"half-negative-small", -0.5, -1},
	{"negative-below-half", -0.4, 0},
	{"near-half", 0.49999999999999994, 0},
	{"zero", 0.0, 0},
	{"large-odd", 4503599627370497.0, 4503599627370497},
	{"large-half-negative", -1000000000000000.5, -1000000000000001},
}

func TestRoundHalfAway(t *testing.T) {
	for _, c := range cases {
		if got := RoundHalfAway(c.x); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}

func TestRoundToEven(t *testing.T) {
	if got := math.RoundToEven(2.5); got != 2 {
		t.Errorf("RoundToEven(2.5) = %v", got)
	}
	if got := math.Floor(-2.5); got != -3 {
		t.Errorf("Floor(-2.5) = %v", got)
	}
	if got := math.Trunc(-2.5); got != -2 {
		t.Errorf("Trunc(-2.5) = %v", got)
	}
}
