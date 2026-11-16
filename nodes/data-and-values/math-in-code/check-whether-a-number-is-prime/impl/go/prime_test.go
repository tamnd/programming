// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

package prime

import (
	"math/big"
	"testing"
)

var cases = []struct {
	name string
	n    int64
	want bool
}{
	{"small-prime", 17, true},
	{"small-composite", 15, false},
	{"even", 100, false},
	{"odd-composite", 91, false},
	{"negative", -7, false},
	{"zero", 0, false},
	{"one", 1, false},
	{"two", 2, true},
	{"three", 3, true},
	{"square-of-prime", 25, false},
	{"large-square", 4293001441, false},
	{"carmichael", 561, false},
	{"large-prime", 1000000007, true},
	{"mersenne-prime", 2147483647, true},
	{"large-composite", 2147483649, false},
}

func TestIsPrime(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := IsPrime(c.n); got != c.want {
				t.Fatalf("IsPrime(%d) = %v, want %v", c.n, got, c.want)
			}
			if c.n >= 0 && big.NewInt(c.n).ProbablyPrime(0) != c.want {
				t.Fatalf("ProbablyPrime(%d) differs", c.n)
			}
		})
	}
}
