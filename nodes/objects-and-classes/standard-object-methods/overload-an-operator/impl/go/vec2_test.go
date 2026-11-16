// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

package vec2

import (
	"slices"
	"testing"
)

var cases = []struct {
	name   string
	xs, ys []int64
	want   []int64
}{
	{"two", []int64{1, 3}, []int64{2, 4}, []int64{4, 6}},
	{"three", []int64{1, 2, 3}, []int64{10, 20, 30}, []int64{6, 60}},
	{"empty", []int64{}, []int64{}, []int64{0, 0}},
	{"single", []int64{5}, []int64{-7}, []int64{5, -7}},
	{"negative", []int64{-1, -2}, []int64{-3, -4}, []int64{-3, -7}},
	{"cancel", []int64{3, -3}, []int64{5, -5}, []int64{0, 0}},
	{"limit", []int64{1e9, 1e9, 1e9}, []int64{-1e9, -1e9, -1e9}, []int64{3e9, -3e9}},
}

func TestVectorSum(t *testing.T) {
	for _, c := range cases {
		got, err := VectorSum(c.xs, c.ys)
		if err != nil || !slices.Equal(got, c.want) {
			t.Errorf("case %s: got %v, %v; want %v", c.name, got, err, c.want)
		}
	}
}

func TestOperators(t *testing.T) {
	a, b := Vec2{1, 2}, Vec2{3, 4}
	if a.Add(b) != (Vec2{4, 6}) || a == b || a != (Vec2{1, 2}) {
		t.Error("Add or == gave a wrong result")
	}
	seen := map[Vec2]bool{a: true}
	if !seen[Vec2{1, 2}] {
		t.Error("an equal vector is not the same map key")
	}
}
