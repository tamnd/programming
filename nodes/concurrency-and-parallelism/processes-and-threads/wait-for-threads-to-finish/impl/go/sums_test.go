// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

package sums

import (
	"slices"
	"testing"
)

func TestParallelSums(t *testing.T) {
	var many [][]int64
	var manyWant []int64
	for i := range int64(32) {
		many = append(many, []int64{i, i, i})
		manyWant = append(manyWant, 3*i)
	}
	cases := []struct {
		name   string
		chunks [][]int64
		want   []int64
	}{
		{"two", [][]int64{{1, 2, 3}, {4, 5}}, []int64{6, 9}},
		{"three", [][]int64{{10}, {20, 30}, {-5, 5}}, []int64{10, 50, 0}},
		{"empty", [][]int64{}, []int64{}},
		{"empty-chunk", [][]int64{{}, {7}}, []int64{0, 7}},
		{"single", [][]int64{{42}}, []int64{42}},
		{"negative", [][]int64{{-1, -2}, {-3}}, []int64{-3, -3}},
		{"limit", [][]int64{{1e9, 1e9, 1e9}, {-1e9, -1e9, -1e9}}, []int64{3e9, -3e9}},
		{"many", many, manyWant},
	}
	for _, c := range cases {
		if got := ParallelSums(c.chunks); !slices.Equal(got, c.want) {
			t.Errorf("case %s: got %v, want %v", c.name, got, c.want)
		}
	}
}
