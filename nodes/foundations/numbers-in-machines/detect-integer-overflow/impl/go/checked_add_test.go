// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

package overflow

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
	{"small", 2, 3, 5, true},
	{"negative", -7, 4, -3, true},
	{"beyond-32-bit", 4000000000, 5000000000, 9000000000, true},
	{"max-plus-zero", max, 0, max, true},
	{"max-plus-one", max, 1, 0, false},
	{"max-plus-max", max, max, 0, false},
	{"min-plus-zero", min, 0, min, true},
	{"min-minus-one", min, -1, 0, false},
	{"min-plus-min", min, min, 0, false},
	{"min-plus-max", min, max, -1, true},
	{"max-minus-one", max, -1, max - 1, true},
}

func TestCheckedAdd(t *testing.T) {
	for _, c := range cases {
		got, ok := CheckedAdd(c.a, c.b)
		if got != c.want || ok != c.wantOK {
			t.Errorf("%s: got (%d, %v), want (%d, %v)", c.name, got, ok, c.want, c.wantOK)
		}
	}
}

func TestPlusWraps(t *testing.T) {
	a := int64(max)
	if a+1 != min {
		t.Errorf("max + 1 = %d, want %d", a+1, int64(min))
	}
}
