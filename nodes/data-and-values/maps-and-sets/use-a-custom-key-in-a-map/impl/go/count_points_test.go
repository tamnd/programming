// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

package countpoints

import (
	"slices"
	"testing"
)

var cases = []struct {
	name   string
	points []Point
	want   []Count
}{
	{"clicks", []Point{{1, 2}, {3, 4}, {1, 2}}, []Count{{Point{1, 2}, 2}, {Point{3, 4}, 1}}},
	{"swap", []Point{{2, 1}, {1, 2}}, []Count{{Point{1, 2}, 1}, {Point{2, 1}, 1}}},
	{"empty", []Point{}, []Count{}},
	{"one", []Point{{0, 0}}, []Count{{Point{0, 0}, 1}}},
	{"negative", []Point{{-1, 0}, {0, -1}, {-1, 0}}, []Count{{Point{-1, 0}, 2}, {Point{0, -1}, 1}}},
	{"joined-digits", []Point{{1, 23}, {12, 3}}, []Count{{Point{1, 23}, 1}, {Point{12, 3}, 1}}},
	{"all-same", []Point{{5, 5}, {5, 5}, {5, 5}}, []Count{{Point{5, 5}, 3}}},
}

func TestCountPoints(t *testing.T) {
	for _, c := range cases {
		if got := CountPoints(c.points); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
