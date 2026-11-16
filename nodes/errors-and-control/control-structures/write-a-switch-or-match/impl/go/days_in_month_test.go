// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

package daysinmonth

import "testing"

var cases = []struct {
	name  string
	month int
	want  int
}{
	{"january", 1, 31},
	{"february", 2, 28},
	{"april", 4, 30},
	{"july", 7, 31},
	{"november", 11, 30},
	{"december", 12, 31},
	{"zero", 0, 0},
	{"thirteen", 13, 0},
	{"minus-one", -1, 0},
}

func TestDaysInMonth(t *testing.T) {
	for _, c := range cases {
		if got := DaysInMonth(c.month); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}
