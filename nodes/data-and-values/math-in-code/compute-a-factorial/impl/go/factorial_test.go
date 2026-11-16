// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

package factorial

import "testing"

var cases = []struct {
	name string
	n    int64
	want int64
}{
	{"five", 5, 120},
	{"ten", 10, 3628800},
	{"zero", 0, 1},
	{"one", 1, 1},
	{"largest-32-bit", 12, 479001600},
	{"beyond-32-bit", 13, 6227020800},
	{"largest-64-bit", 20, 2432902008176640000},
}

func TestFactorial(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := Factorial(c.n); got != c.want {
				t.Fatalf("Factorial(%d) = %d, want %d", c.n, got, c.want)
			}
			if got := Big(c.n); !got.IsInt64() || got.Int64() != c.want {
				t.Fatalf("Big(%d) = %v", c.n, got)
			}
		})
	}
}

func TestBig(t *testing.T) {
	if got := Big(21).String(); got != "51090942171709440000" {
		t.Fatalf("Big(21) = %s", got)
	}
}
