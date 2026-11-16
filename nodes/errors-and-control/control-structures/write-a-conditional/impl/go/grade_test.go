// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

package grade

import "testing"

var cases = []struct {
	name  string
	score int
	want  string
}{
	{"ninety-five", 95, "A"},
	{"eighty-five", 85, "B"},
	{"seventy-five", 75, "C"},
	{"sixty-five", 65, "D"},
	{"forty", 40, "F"},
	{"hundred", 100, "A"},
	{"zero", 0, "F"},
	{"ninety", 90, "A"},
	{"eighty-nine", 89, "B"},
	{"sixty", 60, "D"},
	{"fifty-nine", 59, "F"},
}

func TestGrade(t *testing.T) {
	for _, c := range cases {
		if got := Grade(c.score); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
