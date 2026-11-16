// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

package brackets

import (
	"strings"
	"testing"
)

var cases = []struct {
	name string
	text string
	want bool
}{
	{"pair", "()", true},
	{"nested", "{[()()]}", true},
	{"code", "f(a[0], {b: 1})", true},
	{"crossed", "([)]", false},
	{"empty", "", true},
	{"no-brackets", "abc", true},
	{"unclosed", "((", false},
	{"extra-close", "())", false},
	{"close-first", ")(", false},
	{"wrong-kind", "(]", false},
	{"deep", strings.Repeat("(", 1000) + strings.Repeat(")", 1000), true},
}

func TestBalanced(t *testing.T) {
	for _, c := range cases {
		if got := Balanced(c.text); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
