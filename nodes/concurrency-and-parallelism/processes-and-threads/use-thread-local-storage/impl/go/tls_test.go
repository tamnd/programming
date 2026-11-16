// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

package tls

import (
	"slices"
	"testing"
)

func TestCountPerThread(t *testing.T) {
	var many []int
	for n := 1; n <= 16; n++ {
		many = append(many, n)
	}
	cases := []struct {
		name   string
		counts []int
	}{
		{"three-threads", []int{1, 2, 3}},
		{"uneven", []int{10, 0, 5, 1000}},
		{"empty", []int{}},
		{"zero", []int{0}},
		{"one-thread", []int{7}},
		{"equal-counts", []int{100, 100, 100, 100}},
		{"many-threads", many},
		{"large", []int{100000, 100000}},
	}
	for _, c := range cases {
		if got := CountPerThread(c.counts); !slices.Equal(got, c.counts) {
			t.Errorf("case %s: got %v, want %v", c.name, got, c.counts)
		}
	}
}

func TestCallerCounterDoesNotChange(t *testing.T) {
	var c Counter
	for range 3 {
		c.Bump()
	}
	CountPerThread([]int{5, 6})
	if got := c.Current(); got != 3 {
		t.Errorf("got %d, want 3", got)
	}
}
