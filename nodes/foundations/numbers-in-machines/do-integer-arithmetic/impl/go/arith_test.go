// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

package arith

import "testing"

var cases = []struct {
	name string
	a, b int64
	n    int
	want [6]int64
}{
	{"small", 7, 2, 3, [6]int64{9, 5, 14, 3, 1, 343}},
	{"exact-division", 20, 5, 2, [6]int64{25, 15, 100, 4, 0, 400}},
	{"negative-dividend", -7, 2, 3, [6]int64{-5, -9, -14, -3, -1, -343}},
	{"negative-divisor", 7, -2, 2, [6]int64{5, 9, -14, -3, 1, 49}},
	{"both-negative", -7, -2, 1, [6]int64{-9, -5, 14, 3, -1, -7}},
	{"zero-dividend", 0, 5, 0, [6]int64{5, -5, 0, 0, 0, 1}},
	{"zero-power", 5, 3, 0, [6]int64{8, 2, 15, 1, 2, 1}},
	{"divisor-larger", 3, 7, 1, [6]int64{10, -4, 21, 0, 3, 3}},
	{"largest", 100000, 100000, 3, [6]int64{200000, 0, 10000000000, 1, 0, 1000000000000000}},
	{"smallest", -100000, 1, 3, [6]int64{-99999, -100001, -100000, -100000, 0, -1000000000000000}},
}

func TestArith(t *testing.T) {
	for _, c := range cases {
		if got := Arith(c.a, c.b, c.n); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestCaretIsNotAPower(t *testing.T) {
	if 2^3 != 1 {
		t.Error("2 ^ 3 should be 1")
	}
}
