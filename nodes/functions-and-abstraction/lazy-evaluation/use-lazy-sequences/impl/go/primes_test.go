// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

package primes

import (
	"slices"
	"testing"
)

var cases = []struct {
	name string
	n    int
	want []int
}{
	{"five", 5, []int{2, 3, 5, 7, 11}},
	{"ten", 10, []int{2, 3, 5, 7, 11, 13, 17, 19, 23, 29}},
	{"twenty-five", 25, []int{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97}},
	{"zero", 0, []int{}},
	{"one", 1, []int{2}},
	{"two", 2, []int{2, 3}},
	{"past-a-gap", 31, []int{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127}},
}

func TestFirst(t *testing.T) {
	for _, c := range cases {
		if got := First(c.n); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestReadsOnlyWhatItNeeds(t *testing.T) {
	last := 0
	counted := func(yield func(int) bool) {
		for v := range Naturals(2) {
			last = v
			if !yield(v) {
				return
			}
		}
	}
	got := []int{}
	for p := range Filter(counted, IsPrime) {
		got = append(got, p)
		if len(got) == 5 {
			break
		}
	}
	if last != 11 {
		t.Errorf("the pipeline read up to %d, want 11", last)
	}
}
