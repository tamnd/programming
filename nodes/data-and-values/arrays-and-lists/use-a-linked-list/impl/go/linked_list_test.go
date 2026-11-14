// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

package uselinkedlist

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name string
	ops  []string
	want []int
}{
	{"deque", []string{"push_back 1", "push_back 2", "push_front 0", "pop_back", "push_back 3"}, []int{0, 1, 3}},
	{"remove-middle", []string{"push_back 1", "push_back 2", "push_back 3", "remove 2"}, []int{1, 3}},
	{"empty", []string{}, []int{}},
	{"push-pop", []string{"push_back 1", "pop_front"}, []int{}},
	{"single", []string{"push_front 7"}, []int{7}},
	{"remove-only", []string{"push_back 5", "remove 5", "push_back 6"}, []int{6}},
	{"remove-head", []string{"push_back 1", "push_back 2", "remove 1", "push_front 0"}, []int{0, 2}},
	{"remove-tail", []string{"push_back 1", "push_back 2", "remove 2", "push_back 3"}, []int{1, 3}},
	{"pop-both-ends", []string{"push_back 1", "push_back 2", "pop_front", "pop_back", "push_front 3"}, []int{3}},
	{"remove-absent", []string{"push_back 1", "remove 9"}, []int{1}},
	{"duplicates", []string{"push_back 4", "push_back 5", "push_back 4", "remove 4"}, []int{5, 4}},
	{"push-front-order", []string{"push_front 1", "push_front 2", "push_front 3"}, []int{3, 2, 1}},
	{"negative", []string{"push_front -1", "push_back -2", "remove -1"}, []int{-2}},
	{"limit", []string{"push_back 2147483647", "push_front -2147483648", "remove 2147483647", "push_back 2147483647"}, []int{math.MinInt32, 2147483647}},
}

func TestRunOps(t *testing.T) {
	for _, c := range cases {
		if got := RunOps(c.ops); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
