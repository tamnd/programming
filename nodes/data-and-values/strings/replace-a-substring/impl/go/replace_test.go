// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

package replace

import "testing"

type replaceCase struct {
	name, s, old, nw, want string
}

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
var all = []replaceCase{
	{"all", "a-b-c", "-", "+", "a+b+c"},
	{"word", "Hello, world", "world", "there", "Hello, there"},
	{"none", "abc", "x", "y", "abc"},
	{"empty-s", "", "a", "b", ""},
	{"delete", "a-b-c", "-", "", "abc"},
	{"new-contains-old", "aXa", "X", "XXX", "aXXXa"},
	{"overlap", "aaaa", "aa", "b", "bb"},
	{"overlap-odd", "aaa", "aa", "b", "ba"},
	{"case-sensitive", "Hello hello", "hello", "bye", "Hello bye"},
	{"non-ascii", "naïve caf\u00e9", "\u00e9", "e", "naïve cafe"},
	{"astral", "a\U0001f642b\U0001f642", "\U0001f642", "!", "a!b!"},
	{"combining", "cafe\u0301", "e", "E", "cafE\u0301"},
	{"precomposed", "caf\u00e9", "e", "E", "caf\u00e9"},
	{"dot", "a.b.c", ".", "-", "a-b-c"},
	{"dollar", "price", "price", "$&!", "$&!"},
	{"ampersand", "a-b", "-", "&", "a&b"},
	{"backslash", "a/b", "/", "\\", "a\\b"},
}

var first = []replaceCase{
	{"first-only", "a-b-c", "-", "+", "a+b-c"},
	{"first-none", "abc", "x", "y", "abc"},
	{"first-overlap", "aaa", "aa", "b", "ba"},
	{"first-astral", "\U0001f642\U0001f642", "\U0001f642", "x", "x\U0001f642"},
	{"first-dollar", "a.b", ".", "$&", "a$&b"},
}

func TestReplace(t *testing.T) {
	for _, c := range all {
		if got := Replace(c.s, c.old, c.nw); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestReplaceFirst(t *testing.T) {
	for _, c := range first {
		if got := ReplaceFirst(c.s, c.old, c.nw); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
