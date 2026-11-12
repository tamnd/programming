// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

package substring

import "testing"

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
func TestSubstring(t *testing.T) {
	cases := []struct {
		name       string
		s          string
		start, end int
		want       string
	}{
		{"middle", "Hello, world", 7, 12, "world"},
		{"prefix", "Hello, world", 0, 5, "Hello"},
		{"one-char", "abc", 1, 2, "b"},
		{"empty", "", 0, 0, ""},
		{"whole", "abc", 0, 3, "abc"},
		{"empty-range", "abc", 1, 1, ""},
		{"at-end", "abc", 3, 3, ""},
		{"non-ascii", "naïve café", 2, 5, "ïve"},
		{"cjk", "日本語テキスト", 3, 7, "テキスト"},
		{"astral", "a\U0001F642b\U0001F643c", 1, 4, "\U0001F642b\U0001F643"},
		{"astral-only", "\U0001F642\U0001F643", 1, 2, "\U0001F643"},
		{"combining", "e\u0301t\u00e9", 0, 1, "e"},
		{"combining-mark", "e\u0301t\u00e9", 1, 3, "\u0301t"},
	}
	for _, c := range cases {
		if got := Substring(c.s, c.start, c.end); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
