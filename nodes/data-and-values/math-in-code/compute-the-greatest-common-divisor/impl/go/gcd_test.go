// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

package gcd

import (
	"math/big"
	"testing"
)

var cases = []struct {
	name     string
	a, b     int64
	gcd, lcm int64
}{
	{"common-factor", 12, 18, 6, 36},
	{"coprime", 17, 5, 1, 85},
	{"euclid-example", 48, 180, 12, 720},
	{"zero-and-number", 0, 7, 7, 0},
	{"both-zero", 0, 0, 0, 0},
	{"negative", -12, 18, 6, 36},
	{"both-negative", -4, -6, 2, 12},
	{"equal", 9, 9, 9, 9},
	{"one", 1, 1000000007, 1, 1000000007},
	{"multiple", 5, 25, 5, 25},
	{"fibonacci", 832040, 514229, 1, 427859097160},
	{"big-lcm", 3000000000000000000, 6000000000000000000, 3000000000000000000, 6000000000000000000},
	{"largest", 9223372036854775807, -9223372036854775807, 9223372036854775807, 9223372036854775807},
}

func TestGcdAndLcm(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			for _, p := range [][2]int64{{c.a, c.b}, {c.b, c.a}} {
				gcd, lcm := GcdAndLcm(p[0], p[1])
				if gcd != c.gcd || lcm != c.lcm {
					t.Fatalf("GcdAndLcm(%d, %d) = (%d, %d), want (%d, %d)", p[0], p[1], gcd, lcm, c.gcd, c.lcm)
				}
			}
			var z big.Int
			z.GCD(nil, nil, big.NewInt(c.a), big.NewInt(c.b))
			if !z.IsInt64() || z.Int64() != c.gcd {
				t.Fatalf("big.Int GCD = %v", &z)
			}
		})
	}
}
