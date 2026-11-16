// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

package division

import (
	"math"
	"testing"
)

const (
	max = math.MaxInt64
	min = math.MinInt64
)

var cases = []struct {
	name   string
	a, b   int64
	want   int64
	wantOK bool
}{
	{"exact", 12, 4, 3, true},
	{"remainder-dropped", 7, 2, 3, true},
	{"zero-dividend", 0, 5, 0, true},
	{"zero-divisor", 5, 0, 0, false},
	{"negative-by-zero", -5, 0, 0, false},
	{"zero-by-zero", 0, 0, 0, false},
	{"negative-dividend", -7, 2, -3, true},
	{"negative-divisor", 7, -2, -3, true},
	{"both-negative", -7, -2, 3, true},
	{"min-by-minus-one", min, -1, 0, false},
	{"min-by-one", min, 1, min, true},
	{"max-by-minus-one", max, -1, -max, true},
	{"min-by-max", min, max, -1, true},
}

func TestSafeDiv(t *testing.T) {
	for _, c := range cases {
		got, ok := SafeDiv(c.a, c.b)
		if got != c.want || ok != c.wantOK {
			t.Errorf("%s: got (%d, %v), want (%d, %v)", c.name, got, ok, c.want, c.wantOK)
		}
	}
}

func TestZeroDivisorPanics(t *testing.T) {
	defer func() {
		if recover() == nil {
			t.Error("no panic")
		}
	}()
	var zero int64
	_ = int64(5) / zero
}
