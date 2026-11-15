// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

package rectangle

import (
	"slices"
	"testing"
)

var cases = []struct {
	name                   string
	width, height, factor  int
	want                   []int
}{
	{"three-by-four", 3, 4, 2, []int{12, 14, 48, 28}},
	{"square", 5, 5, 3, []int{25, 20, 225, 60}},
	{"zero-side", 0, 7, 2, []int{0, 14, 0, 28}},
	{"zero-factor", 3, 4, 0, []int{12, 14, 0, 0}},
	{"identity", 3, 4, 1, []int{12, 14, 12, 14}},
	{"limit", 1000, 1000, 46, []int{1000000, 4000, 2116000000, 184000}},
}

func TestReport(t *testing.T) {
	for _, c := range cases {
		if got := Report(c.width, c.height, c.factor); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestInstancesHaveTheirOwnFields(t *testing.T) {
	a := New(1, 2)
	b := New(1, 2)
	a.Scale(3)
	if a.String() != "Rectangle(3, 6)" || b.String() != "Rectangle(1, 2)" {
		t.Errorf("got %v and %v", a, b)
	}
}
