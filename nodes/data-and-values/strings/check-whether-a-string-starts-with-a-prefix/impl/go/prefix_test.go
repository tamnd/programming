// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

package prefix

import "testing"

type prefixCase struct {
	name, s, p string
	want       bool
}

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
var starts = []prefixCase{
	{"yes", "Hello, world", "Hello", true},
	{"no", "Hello, world", "world", false},
	{"whole", "abc", "abc", true},
	{"empty-prefix", "abc", "", true},
	{"empty-s", "", "a", false},
	{"both-empty", "", "", true},
	{"longer-prefix", "ab", "abc", false},
	{"case-sensitive", "Hello", "hello", false},
	{"non-ascii", "caf\u00e9 au lait", "caf\u00e9", true},
	{"cjk", "日本語", "日本", true},
	{"astral", "\U0001f642\U0001f643", "\U0001f642", true},
	{"astral-no", "\U0001f642", "\U0001f643", false},
	{"combining", "cafe\u0301", "cafe", true},
	{"precomposed", "caf\u00e9", "cafe", false},
}

var ends = []prefixCase{
	{"ends-yes", "report.txt", ".txt", true},
	{"ends-no", "report.txt", ".csv", false},
	{"ends-empty", "abc", "", true},
	{"ends-longer", "txt", "a.txt", false},
	{"ends-case", "A.TXT", ".txt", false},
	{"ends-astral", "a\U0001f642", "\U0001f642", true},
	{"ends-combining", "cafe\u0301", "e", false},
}

func TestStartsWith(t *testing.T) {
	for _, c := range starts {
		if got := StartsWith(c.s, c.p); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestEndsWith(t *testing.T) {
	for _, c := range ends {
		if got := EndsWith(c.s, c.p); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
