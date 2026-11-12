// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

package numberline

import (
	"strings"
	"testing"
)

func TestExact(t *testing.T) {
	cases := []struct {
		name string
		n    int
		want string
	}{
		{"zero", 0, ""},
		{"one", 1, "1"},
		{"three", 3, "1,2,3"},
		{"five", 5, "1,2,3,4,5"},
		{"seven", 7, "1,2,3,4,5,6,7"},
		{"nine", 9, "1,2,3,4,5,6,7,8,9"},
		{"ten", 10, "1,2,3,4,5,6,7,8,9,10"},
	}
	for _, c := range cases {
		if got := NumberLine(c.n); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

// Long results: the length, the start and the end.
func TestLong(t *testing.T) {
	cases := []struct {
		name    string
		n, size int
		end     string
	}{
		{"hundred", 100, 291, ",99,100"},
		{"hundred-and-one", 101, 295, ",100,101"},
		{"thousand", 1000, 3892, ",999,1000"},
		{"large", 100000, 588894, ",99999,100000"},
	}
	for _, c := range cases {
		got := NumberLine(c.n)
		if len(got) != c.size || !strings.HasPrefix(got, "1,2,3,") || !strings.HasSuffix(got, c.end) {
			t.Errorf("%s: got a length of %d", c.name, len(got))
		}
	}
}
