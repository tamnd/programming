// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

package point

import (
	"slices"
	"testing"
)

var cases = []struct {
	name         string
	x, y, dx, dy int
	want         []int
}{
	{"right-and-up", 1, 2, 3, 4, []int{4, 6, 7, 10}},
	{"mixed-signs", 5, -1, -2, 3, []int{3, 2, 5, 5}},
	{"zero-move", 7, -3, 0, 0, []int{7, -3, 0, 10}},
	{"negative", -2, -5, -3, -1, []int{-5, -6, 4, 11}},
	{"origin", 2, 3, -2, -3, []int{0, 0, 5, 0}},
	{"limit", 100000000, -100000000, 100000000, -100000000, []int{200000000, -200000000, 200000000, 400000000}},
}

func TestReport(t *testing.T) {
	for _, c := range cases {
		if got := Report(c.x, c.y, c.dx, c.dy); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestTranslateReturnsAnotherPoint(t *testing.T) {
	p := Point{1, 2}
	q := p.Translate(3, 4)
	if p != (Point{1, 2}) || q != (Point{4, 6}) {
		t.Errorf("got %v and %v", p, q)
	}
}
