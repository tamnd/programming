// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

package seconds

import "testing"

var cases = []struct {
	name string
	days int64
	want int64
}{
	{"one-day", 1, 86400},
	{"one-week", 7, 604800},
	{"one-year", 365, 31536000},
	{"zero", 0, 0},
	{"two-days", 2, 172800},
	{"leap-year", 366, 31622400},
	{"last-32-bit-day", 24855, 2147472000},
	{"past-32-bits", 24856, 2147558400},
	{"million-days", 1000000, 86400000000},
}

func TestSecondsIn(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := SecondsIn(c.days); got != c.want {
				t.Fatalf("SecondsIn(%d) = %d, want %d", c.days, got, c.want)
			}
		})
	}
}

func TestSecondsPerDay(t *testing.T) {
	if SecondsPerDay != 86400 {
		t.Fatalf("SecondsPerDay = %d, want 86400", SecondsPerDay)
	}
}
