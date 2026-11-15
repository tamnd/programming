// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

package countto

import (
	"slices"
	"testing"
)

var cases = []struct {
	name string
	n    int
	want []int
}{
	{"ten", 10, []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}},
	{"three", 3, []int{1, 2, 3}},
	{"zero", 0, []int{}},
	{"one", 1, []int{1}},
	{"two", 2, []int{1, 2}},
}

func TestCountTo(t *testing.T) {
	for _, c := range cases {
		if got := CountTo(c.n); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
