// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

package createarray

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	n     int
	value int
	want  []int
}{
	{"three-sevens", 3, 7, []int{7, 7, 7}},
	{"five-zeros", 5, 0, []int{0, 0, 0, 0, 0}},
	{"empty", 0, 7, []int{}},
	{"single", 1, 42, []int{42}},
	{"zero", 2, 0, []int{0, 0}},
	{"negative", 2, -1, []int{-1, -1}},
	{"max-int", 2, 2147483647, []int{2147483647, 2147483647}},
	{"min-int", 2, -2147483648, []int{-2147483648, -2147483648}},
	{"ten", 10, 1, []int{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
}

func TestFilled(t *testing.T) {
	for _, c := range cases {
		if got := Filled(c.n, c.value); len(got) != c.n || !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
