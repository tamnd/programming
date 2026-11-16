// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

package composition

import (
	"strings"
	"testing"
)

func times(names []string, n int) []string {
	var out []string
	for i := 0; i < n; i++ {
		out = append(out, names...)
	}
	return out
}

var cases = []struct {
	name    string
	ops     []string
	x, want int
}{
	{"inc-after-double", []string{"inc", "double"}, 3, 7},
	{"double-after-inc", []string{"double", "inc"}, 3, 8},
	{"three-functions", []string{"negate", "dec", "double"}, 5, -9},
	{"empty", []string{}, 42, 42},
	{"single-zero", []string{"negate"}, 0, 0},
	{"negative", []string{"dec", "negate"}, -5, 4},
	{"low", []string{"inc"}, -1000, -999},
	{"duplicates", []string{"inc", "inc", "inc"}, 7, 10},
	{"limit-high", times([]string{"double"}, 20), 1000, 1048576000},
	{"limit-low", times([]string{"double"}, 20), -1000, -1048576000},
	{"mixed-20", times([]string{"double", "inc"}, 10), 1000, 1026046},
	{"mixed-20-swapped", times([]string{"inc", "double"}, 10), 1000, 1025023},
}

func TestComposeAll(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := ComposeAll(c.ops, c.x); got != c.want {
				t.Fatalf("got %d, want %d", got, c.want)
			}
		})
	}
}

func TestComposeIsGeneric(t *testing.T) {
	exclaim := func(s string) string { return s + "!" }
	shout := Compose(exclaim, strings.ToUpper, strings.TrimSpace)
	if got := shout("  hello "); got != "HELLO!" {
		t.Fatalf("got %q, want %q", got, "HELLO!")
	}
}

func TestComposeOfNothingIsTheIdentity(t *testing.T) {
	if got := Compose[string]()("same"); got != "same" {
		t.Fatalf("got %q, want %q", got, "same")
	}
}
