// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

package take

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	n     int
	want  []int
}{
	{"first-three", []int{1, 2, 3, 4, 5}, 3, []int{1, 2, 3}},
	{"first-two", []int{10, 20, 30, 40}, 2, []int{10, 20}},
	{"zero", []int{1, 2}, 0, []int{}},
	{"empty", []int{}, 3, []int{}},
	{"more-than-length", []int{1, 2}, 5, []int{1, 2}},
	{"equal-length", []int{4, 5, 6}, 3, []int{4, 5, 6}},
	{"one", []int{9, 8}, 1, []int{9}},
	{"negative", []int{-1, -2, -3}, 2, []int{-1, -2}},
	{"extremes", []int{-2147483648, 2147483647, 0}, 2, []int{-2147483648, 2147483647}},
}

var infinite = []struct {
	name     string
	start, n int
	want     []int
}{
	{"from-zero", 0, 5, []int{0, 1, 2, 3, 4}},
	{"from-ten", 10, 3, []int{10, 11, 12}},
	{"none", 7, 0, []int{}},
	{"one", 0, 1, []int{0}},
	{"large-start", 999998, 2, []int{999998, 999999}},
}

func TestTake(t *testing.T) {
	for _, c := range cases {
		if got := Take(c.items, c.n); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestTakeNaturals(t *testing.T) {
	for _, c := range infinite {
		if got := TakeNaturals(c.start, c.n); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestTakeReturnsACopy(t *testing.T) {
	items := []int{1, 2, 3}
	Take(items, 2)[0] = 99
	if !slices.Equal(items, []int{1, 2, 3}) {
		t.Errorf("the input changed: %v", items)
	}
}
