// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

package floatcmp

import (
	"math"
	"testing"
)

var cases = []struct {
	name                 string
	a, b, relTol, absTol float64
	want                 bool
}{
	{"equal", 1.5, 1.5, 1e-9, 0.0, true},
	{"sum-of-tenths", 0.30000000000000004, 0.3, 1e-9, 0.0, true},
	{"different", 1.0, 1.1, 1e-9, 0.0, false},
	{"large-values", 10000000000.0, 10000000001.0, 1e-9, 0.0, true},
	{"zero-and-zero", 0.0, 0.0, 0.0, 0.0, true},
	{"one-ulp-apart", 1.0, 1.0000000000000002, 0.0, 0.0, false},
	{"near-zero-relative", 1e-12, 0.0, 1e-9, 0.0, false},
	{"near-zero-absolute", 1e-12, 0.0, 1e-9, 1e-9, true},
	{"opposite-signs", 1e-10, -1e-10, 1e-9, 0.0, false},
	{"opposite-signs-absolute", 1e-10, -1e-10, 1e-9, 1e-9, true},
	{"on-the-boundary", 10.0, 9.0, 0.1, 0.0, true},
	{"swapped-boundary", 9.0, 10.0, 0.1, 0.0, true},
	{"past-the-boundary", 10.0, 8.875, 0.1, 0.0, false},
	{"negative-values", -1.0, -1.0000000001, 1e-9, 0.0, true},
}

func TestIsClose(t *testing.T) {
	for _, c := range cases {
		if got := IsClose(c.a, c.b, c.relTol, c.absTol); got != c.want {
			t.Errorf("%s: got %v", c.name, got)
		}
		if got := IsClose(c.b, c.a, c.relTol, c.absTol); got != c.want {
			t.Errorf("%s swapped: got %v", c.name, got)
		}
	}
}

func TestNonFinite(t *testing.T) {
	nan, inf := math.NaN(), math.Inf(1)
	if IsClose(nan, nan, 1e-9, 0) || !IsClose(inf, inf, 1e-9, 0) ||
		IsClose(inf, -inf, 1e-9, 0) || IsClose(inf, 1e308, 1e-9, 0) {
		t.Error("non-finite values")
	}
}

func TestSumOfTenths(t *testing.T) {
	// The variables make the sum a float64 sum; a sum of two constants is exact.
	x, y := 0.1, 0.2
	if x+y == 0.3 {
		t.Error("0.1 + 0.2 == 0.3")
	}
	if math.Nextafter(1, 2)-1 != 0x1p-52 {
		t.Error("Nextafter")
	}
}
