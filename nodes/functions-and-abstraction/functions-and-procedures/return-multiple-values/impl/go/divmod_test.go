// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

package divmod

import (
	"math"
	"testing"
)

var cases = []struct {
	name       string
	a, b, q, r int32
}{
	{"positive", 17, 5, 3, 2},
	{"exact", 20, 4, 5, 0},
	{"smaller-dividend", 3, 10, 0, 3},
	{"negative-dividend", -17, 5, -4, 3},
	{"negative-divisor", 17, -5, -4, -3},
	{"both-negative", -17, -5, 3, -2},
	{"exact-negative", -20, 4, -5, 0},
	{"small-negative", -3, 10, -1, 7},
	{"zero-dividend", 0, 7, 0, 0},
	{"zero-negative-divisor", 0, -7, 0, 0},
	{"divisor-one", 9, 1, 9, 0},
	{"int-max", math.MaxInt32, 2, 1073741823, 1},
	{"int-min", math.MinInt32, 3, -715827883, 1},
	{"min-by-max", math.MinInt32, math.MaxInt32, -2, 2147483646},
	{"max-by-min", math.MaxInt32, math.MinInt32, -1, -1},
}

func TestDivMod(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			q, r := DivMod(c.a, c.b)
			if q != c.q || r != c.r {
				t.Fatalf("got (%d, %d), want (%d, %d)", q, r, c.q, c.r)
			}
		})
	}
}
