// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

package manhattan

import "testing"

var cases = []struct {
	name string
	p, q [2]int
	want int
}{
	{"first", [2]int{1, 2}, [2]int{4, 6}, 7},
	{"on-a-line", [2]int{0, 0}, [2]int{10, 0}, 10},
	{"down-left", [2]int{5, 5}, [2]int{2, 1}, 7},
	{"mixed-signs", [2]int{3, -1}, [2]int{-2, 4}, 10},
	{"same-point", [2]int{7, 7}, [2]int{7, 7}, 0},
	{"negative", [2]int{-3, -4}, [2]int{-1, -1}, 5},
	{"large", [2]int{1000000, -1000000}, [2]int{-1000000, 1000000}, 4000000},
}

func TestManhattan(t *testing.T) {
	for _, c := range cases {
		if got := Manhattan(c.p, c.q); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}
