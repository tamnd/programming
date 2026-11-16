// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

package pool

import (
	"slices"
	"testing"
)

func TestStepsOnPool(t *testing.T) {
	var many, manyWant []int
	for n := 1; n <= 200; n++ {
		many = append(many, n)
		manyWant = append(manyWant, collatzSteps(int64(n)))
	}
	cases := []struct {
		name    string
		workers int
		numbers []int
		want    []int
	}{
		{"four-jobs", 2, []int{1, 2, 3, 6}, []int{0, 1, 7, 8}},
		{"long-runs", 3, []int{27, 97, 871}, []int{111, 118, 178}},
		{"empty", 2, []int{}, []int{}},
		{"one-worker", 1, []int{7, 8, 9}, []int{16, 3, 19}},
		{"more-workers-than-jobs", 8, []int{5, 6}, []int{5, 8}},
		{"duplicates", 3, []int{27, 27, 27, 1}, []int{111, 111, 111, 0}},
		{"wide-intermediate", 2, []int{113383, 837799}, []int{247, 524}},
		{"many", 4, many, manyWant},
	}
	for _, c := range cases {
		if got := StepsOnPool(c.workers, c.numbers); !slices.Equal(got, c.want) {
			t.Errorf("case %s: got %v, want %v", c.name, got, c.want)
		}
	}
}
