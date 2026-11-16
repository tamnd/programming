// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

package cycle

import (
	"fmt"
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	start string
	steps int
	want  []string
}{
	{"weekdays", "monday", 3, []string{"monday", "tuesday", "wednesday"}},
	{"wrap", "friday", 4, []string{"friday", "saturday", "sunday", "monday"}},
	{"zero-steps", "monday", 0, []string{}},
	{"more-than-a-week", "sunday", 9, []string{"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "monday"}},
	{"unknown", "funday", 3, []string{}},
	{"wrong-case", "Friday", 2, []string{}},
	{"number", "3", 2, []string{}},
	{"empty-name", "", 1, []string{}},
}

func TestCycle(t *testing.T) {
	for _, c := range cases {
		if got := Cycle(c.start, c.steps); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestString(t *testing.T) {
	if got := fmt.Sprint(Friday, int(Friday)); got != "friday 4" {
		t.Errorf("got %q", got)
	}
	if got := Day(9).String(); got != "Day(?)" {
		t.Errorf("got %q", got)
	}
}
