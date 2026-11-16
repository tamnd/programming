// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

package shape

import "testing"

var cases = []struct {
	name   string
	kind   string
	sides  []int
	want   int
	wantOK bool
}{
	{"square", "square", []int{3}, 12, true},
	{"rectangle", "rectangle", []int{2, 5}, 14, true},
	{"triangle", "triangle", []int{3, 4, 5}, 12, true},
	{"unknown-kind", "circle", []int{2}, 0, false},
	{"wrong-case", "Square", []int{3}, 0, false},
	{"too-few-sides", "rectangle", []int{4}, 0, false},
	{"too-many-sides", "square", []int{3, 3}, 0, false},
	{"no-sides", "triangle", []int{}, 0, false},
	{"zero-side", "square", []int{0}, 0, false},
	{"negative-side", "rectangle", []int{-2, 5}, 0, false},
	{"not-a-triangle", "triangle", []int{1, 2, 3}, 0, false},
	{"largest", "triangle", []int{1000000, 1000000, 1000000}, 3000000, true},
}

func TestPerimeter(t *testing.T) {
	for _, c := range cases {
		got, ok := Perimeter(c.kind, c.sides)
		if got != c.want || ok != c.wantOK {
			t.Errorf("%s: got (%d, %v), want (%d, %v)", c.name, got, ok, c.want, c.wantOK)
		}
	}
}

func TestVariantsCarryTheirData(t *testing.T) {
	s, _ := MakeShape("rectangle", []int{2, 5})
	if r, ok := s.(Rectangle); !ok || r != (Rectangle{2, 5}) {
		t.Errorf("got %#v", s)
	}
}
