// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

package roundrobin

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	work  []int
	slice int
	want  []int
}{
	{"example", []int{3, 1, 2}, 1, []int{1, 2, 0}},
	{"slice-two", []int{4, 1, 3, 2}, 2, []int{1, 3, 0, 2}},
	{"empty", []int{}, 3, []int{}},
	{"one", []int{7}, 2, []int{0}},
	{"big-slice", []int{3, 1, 2}, 10, []int{0, 1, 2}},
	{"slice-one", []int{2, 2, 1}, 1, []int{2, 0, 1}},
	{"equal-work", []int{2, 2, 2}, 1, []int{0, 1, 2}},
	{"long-job-first", []int{10, 1, 1}, 3, []int{1, 2, 0}},
}

func TestFinishOrder(t *testing.T) {
	for _, c := range cases {
		before := slices.Clone(c.work)
		if got := FinishOrder(c.work, c.slice); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if !slices.Equal(c.work, before) {
			t.Errorf("%s: the input changed", c.name)
		}
	}
}
