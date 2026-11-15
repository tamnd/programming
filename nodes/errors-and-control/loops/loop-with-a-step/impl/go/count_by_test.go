// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

package countby

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name              string
	start, stop, step int32
	want              []int32
}{
	{"by-two", 2, 8, 2, []int32{2, 4, 6, 8}},
	{"down", 10, 0, -1, []int32{10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}},
	{"uneven", 1, 10, 3, []int32{1, 4, 7, 10}},
	{"past-stop", 0, 9, 4, []int32{0, 4, 8}},
	{"down-by-three", 10, 1, -3, []int32{10, 7, 4, 1}},
	{"empty-up", 5, 1, 1, []int32{}},
	{"empty-down", 1, 5, -1, []int32{}},
	{"single", 3, 3, 7, []int32{3}},
	{"negative", -3, 3, 2, []int32{-3, -1, 1, 3}},
	{"limit-up", 2147483640, math.MaxInt32, 5, []int32{2147483640, 2147483645}},
	{"limit-down", -2147483640, math.MinInt32, -5, []int32{-2147483640, -2147483645}},
	{"big-step", 0, 10, math.MaxInt32, []int32{0}},
}

func TestCountBy(t *testing.T) {
	for _, c := range cases {
		if got := CountBy(c.start, c.stop, c.step); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
