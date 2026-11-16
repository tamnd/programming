// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

package higherorder

import "testing"

var cases = []struct {
	name               string
	start, times, want int
}{
	{"one-step", 6, 1, 3},
	{"three-steps", 6, 3, 5},
	{"ten-steps", 27, 10, 214},
	{"zero-times", 7, 0, 7},
	{"start-one", 1, 1, 4},
	{"cycle", 1, 3, 1},
	{"past-one", 16, 10, 1},
	{"peak-27", 27, 77, 9232},
	{"peak-9663", 9663, 48, 27114424},
	{"limit", 10000, 1000, 2},
}

func TestCollatzAfter(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := CollatzAfter(c.start, c.times); got != c.want {
				t.Fatalf("got %d, want %d", got, c.want)
			}
		})
	}
}

func TestRepeatTakesAFunctionLiteral(t *testing.T) {
	double := func(x int) int { return x * 2 }
	if got := Repeat(double, 10)(1); got != 1024 {
		t.Fatalf("got %d, want 1024", got)
	}
}

func TestRepeatZeroTimesIsTheIdentity(t *testing.T) {
	if got := Repeat(CollatzStep, 0)(27); got != 27 {
		t.Fatalf("got %d, want 27", got)
	}
}
